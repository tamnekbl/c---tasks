#include <vector>
#include <algorithm>
#include <algorithm>
#include <iostream>
#include <numeric>

using namespace std;


void output(vector<int>::iterator it_begin, vector<int>::iterator it_end)
{
    for(auto i = it_begin; i < it_end; i++) { cout << *i << ' '; };
}

int main(){
    vector<int> vec = {8, 4, 5, 9};
    sort(vec.begin(), vec.end());
    output(vec.begin(), vec.end());
}
