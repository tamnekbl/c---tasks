#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
 
int main()
{
    std::vector<int> nums {1, 2, 4, 8, 16};

 
    // Напечатаем vector.
    std::for_each(nums.begin(), nums.end(), [](const int n) { std::cout << n << ' '; });
}