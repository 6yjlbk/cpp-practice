#include <iostream> //потоковый ввод/вывод
#include <algorithm> //поиск, сортировка и тд для работы с алгоритмом
#include <chrono> //для работы со временем
#include <iomanip> //форматирование ввода/вывода
#include <numeric> //числовые алгоритмы
#include <random> //для рандомного поиска
#include <set> 
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>

using Clock = std::chrono::steady_clock; //задание имени Clock для пространства имен (?)

class Metrics
{
    
};

int main()
{
    std::vector<int> linear; //здесь используются алгоритмы поиска из <algorithm>
    std::vector<int> sorted; //здесб могут использоваться элементы бинарного поиска (делит поплам и отсекает ненужную половину)
    std::set<int> stree; //в set используется поиск по бинарному дереву
    std::unordered_set<int> shash; //используются хэш-таблицы
    std::map<int, int> mtree; //то же самое, но поиск по ключам
    std::unordered_map<int, int> mhash;

}