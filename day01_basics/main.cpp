#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>

double square(double x)
{
    return x*x;
}

int nod (int x, int y)
{
    int nod_x;
    int nod_y;
    int nod_num;
    for (int i = 1;i<=std::max(x,y);i++)
    {
        if ( x % i == 0)
        {
            nod_x=i;
        }
        if ( y % i == 0)
        {
            nod_y=i;
        }
        if ( nod_x == nod_y)
        {
            nod_num=nod_x;
        }
    }
    return nod_num;
}


int main()
{
    double a, b, c;
    std::cin >> a >> b >> c;
    double max_num = std::max({a, b, c});
    double min_num = std::min({a, b, c});
    if (max_num !=min_num)
    {
        std::cout << "The maximum number is: " << max_num << std::endl;
        std::cout << "The minimum number is: " << min_num << std::endl;
    }
    else
    {
        std::cout << "All numbers are equal." << std::endl;
    }
    // bubble filter
   std::vector<double> numbers = {a, b, c};
   std::sort(numbers.begin(), numbers.end());
    for (double num: numbers)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    double num1;
    std::cin >> num1;

    // double rslt = square(num1);
    std::cout << square(num1) << std::endl;

    int d, e;
    std::cin >> d >> e;
    std::cout << nod(d, e) << std::endl;

    return 0;
}

