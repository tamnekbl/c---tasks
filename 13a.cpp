#include <chrono>
#include <vector>
#include <thread>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <mutex>
#include <windows.h>

struct Employee {
    std::string name; 
    std::mutex m;
};


void call_employee(Employee & e1, Employee & e2) {
    while (true) {
        std::lock(e1.m, e2.m);
        std::cout << e1.name << " берёт трубку и набирает " << std::endl;
        Sleep(10); 
        std::cout << e2.name << " берёт трубку и отвечает " << std::endl;
        std::cout << "Разговор..." << std::endl;
        Sleep(5000);
        e2.m.unlock();
        e1.m.unlock();
        Sleep(10);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Employee a, b;
    a.name = "Alice";
    b.name = "Bob";
    std::vector<std::thread> threads;
    threads.emplace_back(call_employee, std::ref(a), std::ref(b));
    threads.emplace_back(call_employee, std::ref(b), std::ref(a));
    threads[0].join();
    threads[1].join();

    return 0;
}
