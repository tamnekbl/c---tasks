#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <windows.h>

using namespace std;

vector<string> createDeck() {
    
    vector<string> ranks = { "6", "7", "8", "9", "10", "Валет", "Дама", "Король", "Туз"};
    vector<string> suits = {"Пик", "Черв", "Крест", "Бубен"};
    vector<string> deck;

    for (const auto& rank : ranks) {
        for (const auto& suit : suits) {
            deck.push_back(rank + '-' + suit);
        }
    }

    return deck;
}

void dealCards(const std::vector<std::string>& deck, std::vector<std::vector<std::string>>& players) {
    size_t playerCount = players.size();
    for (size_t i = 0; i < deck.size(); ++i) {
        players[i % 6].push_back(deck[i]);
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    vector<string> deck = createDeck();
    srand(time(0));
    unsigned seed = 0;
    shuffle(deck.begin(), deck.end(), default_random_engine(seed));
    
    // Создание 4 игроков
    vector<vector<string>> players(4);

    // Раздача карт игрокам
    dealCards(deck, players);

    for (size_t i = 0; i < players.size(); ++i) {
        for (const auto& card : players[i]) {
            cout << card << " ";
        }
        cout << endl << endl;
    }
    
    
    
}