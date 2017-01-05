/*
*   The std::future object provides a mechanism to access the result of asynchronous operations. 
*   The result can be retrived only once. Neither std::promise nor std::future can be copied; only moved. 
*
*   std::async returns a std::future object and expects a function parameter which it runs on a seperate thread if std::launch::async flag is passed, 
*   std::launch::deferred flag will run the function in the same thread. 
*
*/


#include <future>
#include <iostream>

using namespace std;

int factorial(std::future<int>& f) {
    int res = 1;
    
    int N = f.get(); 
    for(int i = N; i > 1; i--)
        res *=i;
        
    cout<<"Result is: "<<res<<endl;    
    return res;
}

int main() {
    
    std::promise<int> p;
    std::future<int> f = p.get_future();
    
    //future<int> fu = async(std::launch::defferred, factorial, 4); // Executes factorial function in the same thread ONLY when get() is called. 
    future<int> fu = async(std::launch::async, factorial, std::ref(f)); // Executes factorial function in a child thread (doesn't wait for get() to be called).

    // do something else
    //std::this_thread::sleep_for(chrono::milliseconds(20));
    p.set_value(4);
    int x = fu.get(); // Waits till child thread finishes and returns result. 
    cout<<"Get from child: "<<x<<endl;
    return 0;
}