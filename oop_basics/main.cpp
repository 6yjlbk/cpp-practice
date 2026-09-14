#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
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
    grade_book["biology"]; //ключ создастся. Для int со значением 0
    std::cout<<grade_book["biology"]<<"  "<<grade_book.at("biology")<< std::endl; //равносиьные записи
    std::cout<<"math grade: "<<grade_book["math"]<< std::endl;
    grade_book["math"] = 4; // перезапись имеющегося ключа
    std::cout<<"math grade: "<<grade_book["math"]<< std::endl;
    grade_book.insert({"math",5}); // insert() не перезаписывает переменную
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
}