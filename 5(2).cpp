#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <vector>
#include <windows.h>
#include <random>
#include <condition_variable>

std::vector<std::mutex*> forkEvents;
std::mutex coutMutex; // Мьютекс для синхронизации вывода
std::vector<bool> states;
std::mutex              lockfork;
std::condition_variable state_check;

void Delay()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1000, 1010);
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

bool Waiter(int id)
{
    if (forkEvents[id]->try_lock())
    {
        states [id] = true; //https://nuancesprog.ru/p/6546/?ysclid=lx3cn47gw8233873141
        std::unique_lock<std::mutex> locker(lockfork);
        if (forkEvents[(id + 1) % 5]->try_lock())
        {
            return true;
        }
        else
        {
            forkEvents[id]->unlock();
        }
    }
    return false;
}

void TakeForks(int id)
{
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "Философ " << id << ": Я взял левую вилку" << std::endl;
    std::cout << "Философ " << id << ": Я взял правую вилку и кушаю" << std::endl;
}

void PutForks(int id)
{
    forkEvents[id]->unlock();
    forkEvents[(id + 1) % 5]->unlock();
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Философ " << id << ": Я закончил кушать и положил вилки" << std::endl;
    }
}

void Guest(int id)
{
    while (true)
    {
        Thinking(id);
        while (!Waiter(id))
        {
            // Ожидание, пока вилки не станут доступны
        }
        TakeForks(id);      
        Lunch();
        PutForks(id);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        forkEvents.push_back(new std::mutex);
        states.push_back(false);
    }
    for (int i = 0; i < 5; i++) {
        threads.emplace_back(Guest, i);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    for (auto m : forkEvents) {
        delete m;
    }
    return 0;
}
