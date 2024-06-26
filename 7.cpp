#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <vector>
#include <windows.h>
#include <memory>

std::string names[] = {"Alice", "Bob", "John", "Sam", "Anna"};
std::mutex m;
class Employee {
    public:
        std::string name; 

    Employee(std::string _name)
    {
        name = _name;
    }
};

void call_employee(Employee* e1, Employee* e2) {
    m.lock();
    std::cout << e1->name << " берёт трубку и набирает абоненту " << e2->name << std::endl;
    Sleep(10);
    std::cout << e2->name << " берёт трубку и отвечает " << std::endl;
    std::cout << "Разговор..." << std::this_thread::get_id() << std::endl;
    Sleep(1000);
    m.unlock();
    Sleep(10);
}

std::shared_ptr<Employee> initialize(Employee& emp) {
    return std::make_shared<Employee>(emp); 
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::vector<Employee> emps;
    std::vector<std::thread> threads;

    srand(time(0));

    for (int i = 0; i < 5; ++i) {
        emps.emplace_back(Employee (names[i]));
        //std::cout<<emps[i].name<<" ";
    }

    for (int i = 0; i < 10; i++) {
        int a = rand() % 5;
        int b = rand() % 5;
        while (b == a) {
            b = rand() % 5;
        }
        threads.emplace_back(call_employee, initialize(emps[a]).get(), initialize(emps[b]).get());
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
