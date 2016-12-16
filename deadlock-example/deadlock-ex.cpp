/*
*   Deadlock describes a situation where two or more threads are blocked forever, waiting for each other to unlock. 
*
*   Avoiding deadlock
*   1. Prefer locking single mutex.
*   2. Avoid locking a mutex and then calling a user provided function.
*   3. Use std::lock() along with std::adopt_lock to lock more than one mutex effectively without incurring deadlock.
*   4. Lock the mutexes in the same order. 
*
*   Locking Granularity:
*   - Fine-grained lock: protects small amount of resources.
        * If your locks are too fine-grained, you're more exposed to deadlock.
*   - Coarse-grained lock: protects large amount of resources.
*       * If your locks are too coarse-grained you're not utilizing parallel computing efficiently. 
*/

// This example is very similar to mutex-example
#include <iostream>
#include <thread>
#include <string>
#include <mutex>

using namespace std;

class Logger {
    mutex m;
    mutex m2;
public:
    Logger() {} // Need destructor to close file
    void shared_print(string id, int value) {
        lock(m, m2); // Used to lock an arbitrary amount of mutexes to avoid deadlock
        lock_guard<mutex> locker(m, adopt_lock); // adopt_lock parameter indicates mutex m is already locked and ownership of lock should be transferred.
        lock_guard<mutex> locker2(m2, adopt_lock); 
        cout<<"From "<<id<<": "<<value<<endl;
    }
    void shared_print2(string id, int value) {
        lock_guard<mutex> locker(m, adopt_lock); 
        lock_guard<mutex> locker2(m2, adopt_lock); 
        cout<<"From "<<id<<": "<<value<<endl;
    }
};

void function_1(Logger& log) {
    for(int i = 0; i > -100; i--) 
        log.shared_print(string("t1: "), i);
}

int main() {
    Logger log;
    thread t1(function_1, ref(log)); // Pass log object by reference to the child thread.
    
    for(int i = 0; i < 100; i++) 
        log.shared_print2(string("main: "), i);
        
    t1.join();
    return 0;
}

