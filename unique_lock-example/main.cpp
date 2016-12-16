/*
*   In this program we look at the third way to lock resources using unique_lock. 
*   unique_lock offers more flexibility and functionality at the cost of performance. 

*   Locking methods (assume we're locking mutex m):
*   1. m.lock(); m.unlock(); // Not recommended
*   2. lock_guard<mutex> lock2(m); // Best performance
*   3. unique_lock<mutex> lock3(m); lock3.unlock(); // unique_lock supports finer grained locking as it supports multiple locks and unlocks. 
*
*/

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>

using namespace std;

class LogFile {
    mutex m;
    std::once_flag fl;
    ofstream f; // f should ONLY be accessible through the mutex.
public:
    LogFile() {} 
    void shared_print(string id, int value) {
        
        // Lazy initialization or Initialization upon first use idiom not efficient when using multiple threads. 
        // unique_lock<mutex> locker2(m_open);
        // if(!f.is_open()) {        
        //     f.open("log.txt");
        // }
        
        call_once(fl, [&](){ f.open("log.txt"); }); // lambda function to ensure the file will be opened only once by one thread 
        
        unique_lock<mutex> locker(m, defer_lock); // std::defer_lock indicates the mutex shouldn't lock till locker.lock() is explicitly called. 
        // Do something that doesn't require ofstream resource...
        
        locker.lock(); // Lock as you need.
        f<<"From "<<id<<": "<<value<<endl;
        locker.unlock(); // Unlock as you need.
        
        unique_lock<mutex> locker2 = move(locker); // Ownership of a unique_lock can be moved to other unique_lock objects. Not possible with lock_guard. 

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