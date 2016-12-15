#include <iostream>
#include <thread>

using namespace std;

class Fctor {
public:
    void operator()(string msg) {
        cout<<"t1 says: "<<msg<<endl;
    }
};

int main() {
    
    string s = "Prepare for unforeseen consequences";
    cout<<this_thread::get_id()<<endl; // get unique identifier of current thread.
    // ref(arg) passes value by reference where as move(arg) passes the pointer all together which is more efficient. 
    thread t1((Fctor()), std::move(s)); // execute a functor or function in its own thread: thread t1(Function, ref(arg1), move(arg2), ...);
    cout<<t1.get_id()<<endl; // get unique thread identifier for t1
    
    //t1.detach(); // t1 will run freely on its own (daemonize process).

    t1.join(); // wait for t1 (thread) to finish executing before continuing;
    cout<<"Number of concurrent threads supported: "<<thread::hardware_concurrency()<<endl; // Useful for avoiding performance degradation via oversubscription  

    return 0;
}
