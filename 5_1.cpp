#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <vector>
#include <windows.h>
#include <random>

std::vector<bool> forks (5, false);
std::mutex forkMutex;
std::mutex coutMutex; // Мьютекс для синхронизации вывода

void Delay()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(5000, 5010);
    Sleep(dist(gen));
}

void Thinking(int id)
{
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Философ " << id << ": Я думаю..." << std::endl;
    }
    Delay();
}

void Lunch()
{
    Delay();
}

bool Waiter(int id) {
    std::lock_guard<std::mutex> lock(forkMutex);
    if (!forks[id] && !forks[(id + 4) % 5]) {
        forks[id] = true;
        forks[(id + 4) % 5] = true;
        Delay();
        return true;
    }
    return false;
}

void TakeForks(int id)
{
    while (!Waiter(id)) {
        
    }
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Философ " << id << ": Я взял левую вилку" << std::endl;
        std::cout << "Философ " << id << ": Я взял правую вилку и кушаю" << std::endl;
        
    }

}

void PutForks(int id)
{
    {
        forks[id] = false;
        forks[(id + 4) % 5] = false;
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Философ " << id << ": Я закончил кушать и положил вилки" << std::endl;
    }
}

void Guest(int id)
{
    while (true)
    {
        Thinking(id);
        TakeForks(id);      
        Lunch();
        PutForks(id);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; i++) {
        threads.emplace_back(Guest, i);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}
