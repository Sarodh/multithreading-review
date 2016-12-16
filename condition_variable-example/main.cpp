/*
*   The condition_variable class is a synchronization primitive that can be used to block a thread, 
*   or multiple threads at the same time,until another thread both modifies a shared variable (the condition), 
*   and notifies the condition_variable. condition_variable is primarily used to facilitate inter-thread communication.
*   ONLY unique_lock can be used with condition_variable.
*
*/

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>

using namespace std;

deque<int> q;
mutex m;
condition_variable cond;

void function_1() {
    int count = 10;
    while(count > 0) {
        unique_lock<mutex> locker(m);
        q.push_front(count);
        locker.unlock();
        cond.notify_one(); // Notify one waiting thread, if there is one. 
        cond.notify_all(); // Notify and wake up all the threads.
        this_thread::sleep_for(chrono::seconds(1));
        count--;
    }
}

void function_2() {
    int data = 0;
    while(data != 1) {
        unique_lock<mutex> locker(m);
        cond.wait(locker, []{ return !q.empty(); }); // Unlocks locker then waits for another thread to notify this thread before executing code below. Lambda function is called to verify the condition is met to continue execution.  
        data = q.back();
        q.pop_back();
        locker.unlock();
        cout << "t2 got a value from t1: " << data << endl;
    }
}

int main() {
    thread t1(function_1);
    thread t2(function_2);
    
    t1.join();
    t2.join();
    
    return 0;
}
