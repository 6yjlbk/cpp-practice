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
#include "metric.h"

using Clock = std::chrono::steady_clock; //задание имени Clock для пространства имен (?)

template <typename Container>
void RandInit(Container& container)
{
    std::mt19937 gen(std::random_device{}()); //инициализация генератора, который производит случайные числа, random_device нужен чтобы менять значения перед каждым запуском
    std::uniform_int_distribution<int> dist(0,10000); //распределение чисел по нужном правилу в заданном диапазоне
    for (int i=0; i<3000;++i)
    {
        container.insert(container.end(),dist(gen));
    }
}

template <typename Container>
void RandInitMap(Container& container)
{
    std::mt19937 gen(std::random_device{}()); //инициализация генератора, который производит случайные числа, random_device нужен чтобы менять значения перед каждым запуском
    std::uniform_int_distribution<int> dist(0,10000); //распределение чисел по нужном правилу в заданном диапазоне
    for (int i=0; i<3000;++i)
    {
        container.emplace(i,dist(gen));
    }
}

// шаблон  для вывода контейнеров, где нет ключа
template <typename Container>
void Print(const Container& container)
{
    for (const auto& num : container)
    {
        std::cout<<num<<" ";
    }
    std::cout << '\n';
}

//Шаблон для контейнров типо map, multimap, unordered_map, unordered_multimap
template <typename Container>
void PrintMap(const Container& container)
{
    for (const auto& [key, value] : container )
    {
        std::cout<<key<<" - "<<value<<std::endl;
    }
}


struct Metrics
{
    int hits=0;
    double time=0;
    Metrics(const auto& container, const std::vector<int>& queiries, auto search)
    {
        //вычисление метрик
        const auto start=Clock::now();
        for (int key : queiries)
        if (search(container,key)) //у вектора нет метода .find(), есть только std::find
        {   
            ++hits; //количество успешных поисков +1
        }
        const auto finish=Clock::now();
        time=std::chrono::duration<double, std::milli>(finish-start).count(); // вычисление времени работы в мс, формат double
    }
};

int main()
{
    //Подготовка данных:

    constexpr int N = 3000; //константа, которая известна в момент компиляции, менять нельзя;
    std::vector<int> data(N);
    std::iota(data.begin(), data.end(), 0); //заполнение вектора data 0..2999
    std::mt19937 gen(10); //10 - произвольное число, которое задает начальное состояние генератора
    std::shuffle(data.begin(),data.end(),gen); //перемешивание. gen нужен для того, чтобы выбрать, как переставить уже имеющиеся элементы
    
    // в рез-те была сформирована data с размерностью 3000эл-ов, хранящая в себе в рандомном порядке все эти эл-ты

    std::vector<int> linear; //здесь используются алгоритмы поиска из <algorithm>. вектор длиной N эл-ов     
    std::vector<int> sorted; //здесб могут использоваться элементы бинарного поиска (делит поплам и отсекает ненужную половину)
    std::set<int> stree; //в set используется поиск по бинарному дереву
    std::unordered_set<int> shash; //используются хэш-таблицы
    std::map<int, int> mtree; //то же самое, но поиск по ключам
    std::unordered_map<int, int> mhash; //то же самое, но поиск по ключам

    for (int key:data)
    {
        linear.push_back(key);
        sorted.push_back(key);
        stree.insert(key);
        shash.insert(key);
        mtree.emplace(key,key*10);
        mhash.emplace(key,key*10);
    }
    std::sort(sorted.begin(), sorted.end()); //сортировка вектора

    constexpr int Q=10000; // константа, обозначающая число поисковых запросов
    std::vector<int> queiries;
    queiries.reserve(Q); // резервирование места, без инициализации самих переменных
    std::uniform_int_distribution<int> dist(0,2*N-1); //распределение чисел по нужном правилу в заданном диапазоне. диапазон чисел запрос в два раза больше чем число ключей, чтобы были успешнеые и неуспешные запросы
    for (int i=0; i<Q;++i)
    {
        queiries.push_back(dist(gen));
    }

    // Запуск анализа
    
    Metrics test1 (linear,queiries,LinearSearch);
    Metrics test2 (sorted,queiries,BinarySearch);
    Metrics test3 (mtree,queiries,AssociativeSearch);
    Metrics test4 (mhash,queiries,AssociativeSearch);
    Metrics test5 (stree,queiries,AssociativeSearch);
    Metrics test6 (shash,queiries,AssociativeSearch);
    std::cout<<"Nime of hits: "<<test1.hits<<"; Time: "<<test1.time<<" ms"<<std::endl;
    std::cout<<"Nime of hits: "<<test2.hits<<"; Time: "<<test2.time<<" ms"<<std::endl;
    std::cout<<"Nime of hits: "<<test3.hits<<"; Time: "<<test3.time<<" ms"<<std::endl;
    std::cout<<"Nime of hits: "<<test4.hits<<"; Time: "<<test4.time<<" ms"<<std::endl;
    std::cout<<"Nime of hits: "<<test5.hits<<"; Time: "<<test5.time<<" ms"<<std::endl;
    std::cout<<"Nime of hits: "<<test6.hits<<"; Time: "<<test6.time<<" ms"<<std::endl;


}