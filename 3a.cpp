#include <vector>
#include <algorithm>
#include <algorithm>
#include <iostream>
#include <numeric>

using namespace std;


void output(vector<int> vec)
{
    for(auto i = vec.begin(); i < vec.end(); i++) { cout << *i << ' '; };
}

int main(){
    vector<int> vec = {8, 4, 5, 9};
    output(vec);
}

