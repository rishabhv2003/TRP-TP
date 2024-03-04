#include <iostream>
#include <vector>

void printVectors()
{
    std::cout << std::endl;
}


template <typename T>
void printVectors(const std::vector<T> &vec)
{
    std::cout << "Vector: ";
    for (const auto &element : vec)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}


template <typename T, typename... Args>
void printVectors(const std::vector<T> &first, const Args &...args)
{
    printVectors(first);   
    printVectors(args...);
}