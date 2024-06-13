#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <vector>
#include <windows.h>
#include <random>
#include <condition_variable>

std::mutex Mutex;

class Account
{
    private:
        int balance;

    public: 
        int GetBalance() {return balance;};
        Account(int _balance)
        {
            balance = _balance;
        }
        void Adjust(Account & acc, int amount)
        {
            Mutex.lock();
            balance += amount;
            acc.balance -= amount; 
            Mutex.unlock();
        }
};


void Delay()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100, 110);
    Sleep(dist(gen));
}

std::vector<bool> state (5, false);
int adjusts [] = {150, -150, -100, 100, -503, -14, 503, 14, 5, 5};

void Update (Account & acc1, Account & acc2)
{
    for (int i : adjusts)
    {
        acc1.Adjust(acc2, i);   
        Delay(); 
    }
}





int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::vector<std::thread> threads;
    Account account1 (1000), account2 (1000);
    for (int i = 0; i < 500; i++) {
        threads.emplace_back(Update, std::ref(account1), std::ref(account2));
        threads.emplace_back(Update, std::ref(account2), std::ref(account1));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    std::cout<<account1.GetBalance()<<std::endl;
    std::cout<<account2.GetBalance()<<std::endl;
    return 0;
}
