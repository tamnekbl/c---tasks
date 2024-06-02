#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <vector>
#include <windows.h>
#include <random>

std::vector<std::mutex*> forkEvents;

void Delay()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1000, 1010);
    Sleep(dist(gen));
}

void Thinking(int id)
{
    std::cout<<"Философ "<<id<<": Я думаю..."<<std::endl;
    Delay();
}

void Lunch()
{
    Delay();
}

void TakeForks(int i)
{
    forkEvents[i]->lock();
    std::cout<<"Философ "<<i<<": Я взял левую вилку"<<std::endl;
    forkEvents[(4+i)%5]->lock();
    std::cout<<"Философ "<<i<<": Я взял правую вилку и кушаю"<<std::endl;
}

void PutForks(int i)
{    
    std::cout<<"Философ "<<i<<": Я закончил кушать и положил вилки"<<std::endl;
    forkEvents[i]->unlock();
    forkEvents[(4+i)%5]->unlock();
}

void Guest(int id)
{   
    while(true)
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
    for (int i = 0; i<5; ++i){
        forkEvents.push_back(new std::mutex);
    } 
    for (int i = 0; i<5; i++){
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
