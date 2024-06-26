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

class Meeting {
public:
    static std::vector<std::shared_ptr<Meeting>> meetings;
    std::thread::id meetID;
    Employee* e1;
    Employee* e2;

    Meeting(std::thread::id ID, Employee* ptr1, Employee* ptr2)
        : meetID(ID), e1(ptr1), e2(ptr2) {
        meetings.push_back(std::make_shared<Meeting>(*this));
    }

    static void printMeetings() {
        for (const auto& meeting : meetings) {
            std::cout << "Meeting ID: " << meeting->meetID << " between " << meeting->e1 << " and " << meeting->e2 << std::endl;
        }
        
    }

};

std::vector<std::shared_ptr<Meeting>> Meeting::meetings;

void call_employee(Employee* e1, Employee* e2) {
    m.lock();
    Meeting meeting (std::this_thread::get_id(), e1, e2);
    std::cout << e1 << " берёт трубку и набирает абоненту " << e2 << std::endl;
    Sleep(10);
    std::cout << e2 << " берёт трубку и отвечает " << std::endl;
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

    Meeting::printMeetings();

    return 0;
}
