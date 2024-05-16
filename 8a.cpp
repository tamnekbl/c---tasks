#include <vector>
#include <algorithm>
#include <algorithm>
#include <iostream>
#include <numeric>

using namespace std;


int sum1(int x, int y)
{
    return x+y;
}

void sum2(int *x, int *y)
{
    *x=*x+*y;
}

void sum3(int &x, int &y)
{
    x=x+y;
}

int main(){
    int a = 10;
    int b = -4;
    sum2(&a, &b);
    cout<<a;
}
