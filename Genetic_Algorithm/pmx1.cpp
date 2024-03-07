#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

vector<int> pmx(const vector<int> &parent1, const vector<int> &parent2, int sliceIndex1, int sliceIndex2)
{
    if (parent1.size() != parent2.size())
    {
        throw runtime_error("Parents have not the same length!");
    }

    vector<int> child1(parent1.begin(), parent1.end());
    vector<int> child2(parent2.begin(), parent2.end());
    unordered_map<int, int> mapping1;
    unordered_map<int, int> mapping2;

    for (int i = sliceIndex1; i < sliceIndex2; i++)
    {
        mapping1[parent2[i]] = parent1[i];
        child1[i] = parent2[i];

        mapping2[parent1[i]] = parent2[i];
        child2[i] = parent1[i];
    }

    for (int i = 0; i < sliceIndex1; i++)
    {
        while (mapping1.find(child1[i]) != mapping1.end())
        {
            child1[i] = mapping1[child1[i]];
        }
        while (mapping2.find(child2[i]) != mapping2.end())
        {
            child2[i] = mapping2[child2[i]];
        }
    }

    for (int i = sliceIndex2, l = child1.size(); i < l; i++)
    {
        while (mapping1.find(child1[i]) != mapping1.end())
        {
            child1[i] = mapping1[child1[i]];
        }
        while (mapping2.find(child2[i]) != mapping2.end())
        {
            child2[i] = mapping2[child2[i]];
        }
    }

    return child1;
}

int main()
{

    vector<int> parent1 = {5, 2, 2, 6, 1, 7, 1, 4, 3, 5};

    vector<int> parent2 = {5, 1, 6, 2, 4, 3, 2, 5, 7, 1};


    int sliceIndex1 = 3;
    int sliceIndex2 = 7;
    
    vector<int> child = pmx(parent1, parent2, sliceIndex1, sliceIndex2);
    // sliceIndex1 = 2;
    // sliceIndex2 = 8;
    vector<int> child1 = pmx(parent1, parent2, sliceIndex1, sliceIndex2);

    cout << "Parent 1: ";
    for (int value : parent1)
    {
        cout << value << " ";
    }
    cout << endl;

    cout << "Parent 2: ";
    for (int value : parent2)
    {
        cout << value << " ";
    }
    cout << endl;

    cout << "Child  1: ";
    for (int value : child)
    {
        cout << value << " ";
    }
    cout << endl;
    cout << "Child  2: ";
    for (int value : child1)
    {
        cout << value << " ";
    }
    cout << endl;

    return 0;
}
