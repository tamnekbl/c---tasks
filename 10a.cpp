#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>


int main()
{
    int sum = 0;
    int p{0};
    std::vector<int> nums (10);
    srand(time(0));

    std::for_each(nums.begin(), nums.end(), [&sum](const int n) {
        sum += rand() % 11;
    });
    
    std::cout<<sum;
    
}