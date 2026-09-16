#pragma once //подключение заголовочного файла только один раз
#include <algorithm>
// inline позволяет несколько раз подключить лямбду - функцию без отдельного имени
//ниже описаны различные варианты поиска.
//linear - обычный проход по всем элементам
inline constexpr auto LinearSearch = [](const auto& container, int key)
{
    return std::find(container.begin(),container.end(),key)!=container.end();
};
//binear - делит пополам и отсекает половину
inline constexpr auto BinarySearch = [](const auto& container, int key)
{
    return std::binary_search(container.begin(),container.end(),key);
};
//associative - встроенный поиск ассоциативных контейнеров 
inline constexpr auto AssociativeSearch = [](const auto& container, int key)
{
    return container.find(key) != container.end();
};
