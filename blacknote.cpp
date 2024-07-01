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
    std::cout << e1 << "     адреса     " << e2 << std::endl;
    Sleep(10);
    std::cout << "Разговор..." << std::this_thread::get_id() << std::endl;
    Sleep(1000);
    m.unlock();
    Sleep(10);
}

std::unique_ptr<Employee> initialize(Employee& emp) {
    std::unique_ptr<Employee> ptr = std::make_unique<Employee> (emp);
    std::cout<<ptr.get()<<" указатель в init ---- "<<ptr.get()->name<<std::endl;
    return ptr; 
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

    for (int i = 0; i < 2; i++) {
        int a = rand() % 5;
        int b = rand() % 5;
        while (b == a) {
            b = rand() % 5;
        }
        Employee * p1 = initialize(emps[a]).get();
        Employee * p2 = initialize(emps[b]).get();
        threads.emplace_back(call_employee, p1, p2);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
