#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <vector>
#include <windows.h>

class Employee {
    public:
        std::string name; 
        std::mutex m;

    Employee(std::string _name)
    {
        name = _name;
    }
};




void Guest(int& id)
{

}
/*
void call_employee(Employee & e1, Employee & e2) {
    std::lock(e1.m, e2.m);
    std::cout << e1.name << " берёт трубку и набирает абоненту " <<e2.name<< std::endl;
    Sleep(10); 
    std::cout << e2.name << " берёт трубку и отвечает " << std::endl;
    std::cout << "Разговор..." <<std::this_thread::get_id()<< std::endl;
    Sleep(1000);
    e2.m.unlock();
    e1.m.unlock();
    Sleep(10);
}

void Operator (std::vector<Employee*> & emps)
{   
    srand(time(0));
    while (true)
    {
        int a = rand()%5;
        int b = rand()%5;
        while (b==a)
        {
            b = rand()%5;
        }
        call_employee(*emps.at(a), *emps.at(b));
    }
    
}

//*/

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::string names [] = {"Alice", "Bob","John", "Sam", "Anna"};
    std::vector<std::mutex*> emps;
    std::vector<std::thread> threads;
    for (int i = 0; i<5; ++i){
        emps.push_back(new std::mutex);
    } 
    for (int i = 0; i<5; i++){
        threads.emplace_back(Operator, std::ref(emps));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    for (auto emp : emps) {
        delete emp;
    }
    return 0;
}
