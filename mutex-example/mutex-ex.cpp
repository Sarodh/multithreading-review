/* 
*   A Race conndition occurs when the outcome of a program depends on the relative 
*   execution order of one or more threads (arbitrary sharing of common resources).
*   The Solution is to use mutex to synchronize the access of the common resource.  
*   In this program we will demonstrate the use of mutex to share resources. 
*   
*   It is NOT recommended to use lock and unlock in case the locked block of code throws an exception which results in the resources being permanently locked.
*   mutex m;
*   m.lock(); // The resources below are locked for other threads till this thread unlocks it.
*   <block of code> // If this code throws an exception; resources will not be unlocked.
*   m.unlock(); // Unlock resources used above.
*
*   RAII allows us to unlock resources upon context switching without having to worry about exceptions permanently locking resources.  
*   lock_guard<mutex> guard(m); // Whenever guard goes out of scope the resources (m in this case) will always be unlocked.
*
*   In the case of locking global resources you should isolate the resource to only be accessed via mutex throughout your entire program.
*
*   Avoiding Data Race:
*   1. Use mutex to syncronize data access.
*   2. Never leak locked resource to the outside world.
*   3. Design interface appropriately.
*/

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>

using namespace std;

class LogFile {
    mutex m;
    ofstream f; // f should ONLY be accessible through the mutex.
public:
    LogFile() {
        f.open("log.txt");
    } // Need destructor to close file
    void shared_print(string id, int value) {
        lock_guard<mutex> locker(m); 
        f<<"From "<<id<<": "<<value<<endl;
    }
};

void logger(LogFile& log) {
    for(int i = 0; i > -100; i--) 
        log.shared_print(string("t1: "), i);
}

int main() {
    LogFile log;
    thread t1(logger, ref(log)); // Pass log object by reference to the child thread.
    
    for(int i = 0; i < 100; i++) 
        log.shared_print(string("main: "), i);
        
    t1.join();
    return 0;
}