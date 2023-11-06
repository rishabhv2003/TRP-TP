#include <bits/stdc++.h>
#include <ctime>

using namespace std;

// function to calculate the ferequency of each tool
void toolFreqMat(map<int, int> &toolFreq, vector<vector<int>> jsm)
{
    for (int i = 0; i < jsm.size(); i++)
    {
        for (int j = 0; j < jsm[i].size(); j++)
        {
            toolFreq[jsm[i][j]]++;
        }
    }
}

// function to generate the initial population.
void intialPopulationGen(vector<vector<int>> &popSize, int totalTools)
{
    // randomising the population.
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, totalTools - 1);
    for (int i = 0; i < popSize.size(); i++)
    {
        popSize[i] = vector<int>(totalTools);
        for (int j = 0; j < totalTools; j++)
        {
            popSize[i][j] = j;
        }
        for (int j = 0; j < totalTools; j++)
        {
            int m = dis(gen);
            int n = dis(gen);
            swap(popSize[i][m], popSize[i][n]);
        }
    }
}

// function to print the population in a file named pop_file.txt
void printPop(vector<vector<int>> &popSize, vector<int> &fitnessValue, int totalTools)
{
    const string fileName = "pop_file.txt";
    ofstream popFile(fileName);
    if (!popFile.is_open())
    {
        cerr << "Failed to open the file for writing." << endl;
    }
    for (int i = 0; i < popSize.size(); i++)
    {
        for (int j = 0; j < totalTools; j++)
        {
            popFile << popSize[i][j] << ' ';
        }
        popFile << "| " << fitnessValue[i];
        popFile << endl;
    }
    popFile.close();
}

// fitness function
int getElemenetIndex(const vector<int> v, int elm)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (elm == v[i])
            return i;
    }
    return -1;
}

void calcFitness(vector<vector<int>> &popSize, vector<int> &fitnessValue, vector<vector<int>> &jsm)
{
    for (int i = 0; i < popSize.size(); i++)
    {
        int currIndex = 0;
        int cost = 0;
        vector<int> v = popSize[i];
        for (int k = 0; k < jsm.size(); k++)
        {
            for (int l = 0; l < jsm[k].size(); l++)
            {
                int toolIndex = getElemenetIndex(v, jsm[k][l]);
                cost = cost + abs(currIndex - toolIndex);
                currIndex = toolIndex;
            }
        }
        fitnessValue[i] = cost;
    }
}

int main()
{
    vector<int> batch = {20, 30, 20, 15};
    vector<vector<int>> jsm = {
        {6, 4, 0, 2, 9},
        {3, 6, 9, 8, 7},
        {6, 0, 1, 5, 4},
        {1, 2, 0, 4, 3},
    };
    // map to store the number of times a tool is used.
    map<int, int> toolFreq;
    int totalTools;
    // a 2d vector to store initial population.
    vector<vector<int>> popSize(100);

    // vector to store the fitness of each chromosome.
    vector<int> fitnessValue(popSize.size());

    // making tool frequency matrix
    toolFreqMat(toolFreq, jsm);
    totalTools = toolFreq.size();

    intialPopulationGen(popSize, totalTools);

    calcFitness(popSize, fitnessValue, jsm);

    printPop(popSize, fitnessValue, totalTools);

    return 0;
}
