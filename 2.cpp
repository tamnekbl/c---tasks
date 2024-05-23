#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <mutex>
#include<windows.h>

struct Employee
{
    std::string name; 
    std::mutex m;
};

void call_employee(Employee & e1, Employee & e2)
{
    e1.m.lock();
    e2.m.lock();
    std::cout<<"Calling...";
    Sleep(5000);
    e1.m.unlock();
    e2.m.unlock();
}

int main()
{
    Employee a, b;
    a.name = "Alice";
    b.name = "Bob";
    std::thread th1(call_employee, std::ref (a), std::ref (b));
    std::thread th2(call_employee, std::ref (b), std::ref (a));

    th1.join();
    th2.join();
    

    return 0;
}