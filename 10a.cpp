#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>


int main()
{
    static int sum = 0;
    std::vector<int> nums (10);
    srand(time(0));
    std::for_each(nums.begin(), nums.end(), [](const int n) {
        sum += rand() % 11;
    });
    
    std::cout<<sum;
    
}