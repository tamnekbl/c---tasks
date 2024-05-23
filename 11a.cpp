#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <numeric>
#include<windows.h>

    
int main(){

    auto foo = [](){
        for (int i = 0; i<10; ++i){
           std::cout<<i<<" ";
           Sleep(1000);
        }
    };

    std::thread thread(foo);
    thread.join();
}

