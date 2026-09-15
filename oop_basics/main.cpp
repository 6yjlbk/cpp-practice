#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <random>

int main()
{
    // Контейнер map - ключ - значение
    std::cout<<"-----MAP-----"<<std::endl;
    std::map<std::string, int> grade_book;
    grade_book = {{"math", 3}, {"phys",5}}; // начальная инициализация
    grade_book["literature"] = 3; //добавление новой пары ключ-значение
    std::cout<<grade_book.at("math")<< std::endl; //вывод; at() функция для доступа к элементу по ключу
    std::cout<<grade_book.at("math")<< std::endl; //вывод
    auto it = grade_book.find("phys"); //it - итератор, который позволяет обращаться к элементам контейнера, функция find() ищет по ключу
    std::cout<<it->second<<std::endl; //it->first - ключ ; it->second - значение
    auto it1 = grade_book.find("geography");
    if (it1 != grade_book.end()) //end()  специальная позизиця после последнего элемента. если find() вернул end(), то ключ не найден
    {
        std::cout<<it1->first<<' '<<it1->second<< std::endl;
    }
    else
    {
        std::cout<<"No subject"<< std::endl;
    }
    grade_book["biology"]; //ключ создастся. Для int - со значением 0 !НО at() не сможет создат отсуствущий ключ 
    std::cout<<grade_book["biology"]<<"  "<<grade_book.at("biology")<< std::endl; //равносильные записи
    auto removed = grade_book.erase("biology"); //удаляет запись целиком: ключ и значение. возвращает количество удаленных элементов. 1 - запись была и ее удалили; 0 - запись отсуствовал
    auto removed1 = grade_book.erase("history");
    std::cout<<removed<<" "<<removed1<<std::endl; 
    std::cout<<"math grade: "<<grade_book["math"]<< std::endl;
    grade_book["math"] = 4; // перезапись имеющегося ключа
    std::cout<<"math grade: "<<grade_book["math"]<< std::endl;
    grade_book.insert({"math",5}); // !НО insert() не перезаписывает переменную
    std::cout<<"math grade: "<<grade_book["math"]<< std::endl;
    std::cout<<"full map"<<std::endl;
    for (const auto& [name,num] : grade_book)
    {
        std:: cout << name <<" - "<<num<< std::endl; //map обходится в порядке ключей.
    }

    // Контейнер set - только ключ
    std::cout<<"-----SET-----"<<std::endl;
    std::set<int> student_id = {1, 23, 4, 11, 43};
    student_id.insert(5);
    student_id.insert(15);
    student_id.insert(5);
    std::cout<<"full set"<<std::endl;
    for ( const int& id : student_id)
    {
        std::cout<<id <<std::endl; //set обходится в порядке ключей.
    }

    // Контейнер multimap - несколько значений для одного ключа
    std::cout<<"-----MULTIMAP-----"<<std::endl;
    std::multimap<std::string, int> nums;
    nums ={{"Ann", 89}, {"Alex",12}, {"Tom",54}, {"Alex",3}}; // можно добавить несколько значений для одного ключа
    nums.insert({"Ben",46});
    for (const auto& [name,num] : nums)
    {
        std::cout <<name<<" "<<num<<std::endl;
    }
    std::cout <<"--------"<<std::endl; 
    auto it2=nums.find("Alex"); //???? находит только первое значение из списка
    std::cout<<it2->first<<" "<<it2->second<<std::endl;
    // nums.at("Tom") =18; // нельзя обратиться через at, т.к. в multimap у одного ключа может быть несколько значений
    // nums["Tom"]=18; // нельзя обратиться через [], т.к. в multimap у одного ключа может быть несколько значений
    auto remove3=nums.erase("Alex"); //удаляет все элементы с таким ключом
    std::cout<<"num of del elems: "<<remove3<<std::endl;
    std::cout <<"--------"<<std::endl; 
     for (const auto& [name,num] : nums)
    {
        std::cout <<name<<" "<<num<<std::endl;
    }

    // Контейнер multiset - несколько значений ключа
    std::cout<<"-----MULTISET-----"<<std::endl;
    std::multiset<double> dnums;
    dnums={21.2, 7,3, 12, 21.2, 12.65, 65, 3};
    for (const double& num : dnums)
    {
        std::cout<<num<<" ";
    }
    std::cout<<"\nnum of duplicates: "<<dnums.count(21.2)<<std::endl; // возврашает число повторений
    
    auto it4 = dnums.find(21.2);
    if (it4 !=dnums.end())
    {
        dnums.erase(it4); //удаление только одного значения
    }
    std::cout <<"--------"<<std::endl; 
    for (const double& num : dnums)
    {
        std::cout<<num<<" ";
    }

    // Контейнер unordered_map
    std::cout<<"\n-----UNORDERED_MAP-----"<<std::endl;
    std::unordered_map<std::string, double> noname ={{"a",12.2},{"c", 0.43},{"b",1.54},{"fd",3}};
    for (const auto& [name,num] : noname)
    {
        std::cout<<name<<" "<<num<<std::endl; // порядок не регламентируется и зависит от работы самого контейнера
    }
    
    // Оценка скорости работы
    std::cout<<"work time estimation"<<std::endl;
    std::unordered_map<int, int> rndm_umap;
    std::unordered_map<int, int> rndm1_umap;
    std::map<int,int> rndm_map;
    rndm_umap.reserve(3000); //подготавливаются места, но не создаются элементы
    rndm1_umap.reserve(3000);
    std::mt19937 gen(std::random_device{}()); //инициализация генератора, который производит случайные числа, random_device нужен чтобы менять значения перед каждым запуском
    std::uniform_int_distribution<int> dist(0,8000); //распределение чисел по нужном правилу в заданном диапазоне 
    for (int i=0; i<3000;++i)
    {
        rndm_umap.emplace(i,dist(gen)); // добавляет пару ключ-случайно число
        rndm1_umap.emplace(dist(gen),dist(gen));
        rndm_map.emplace(i,dist(gen));
    }
    auto itt1=rndm_umap.find(1143);
    std::cout<<itt1->first<<" "<<itt1->second<<std::endl;
    auto itt2=rndm_map.find(1143);
    std::cout<<itt2->first<<" "<<itt2->second<<std::endl;
    auto itt3=rndm1_umap.find(1143);
    if (itt3 != rndm1_umap.end())
    {
        std::cout<<itt3->first<<' '<<itt3->second<< std::endl;
    }
    else
    {
        std::cout<<"No subject"<< std::endl;
    }
    



}