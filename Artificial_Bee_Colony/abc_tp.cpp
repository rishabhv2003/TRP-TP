#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <ctime>
#include <set>
#include <cstdlib>
#include <cmath>
#include <climits>

using namespace std;

class Bee
{
public:
    vector<int> individual;
    vector<int> toolLife;
    int fitness;

    Bee() : fitness(0) {}
};

const int totalBees = 50;
const int maxGenerations = 100;
const int productQuantity = 300;
int totalTools;

vector<Bee> colony(totalBees);
Bee bestBee;
unordered_map<int, int> toolLifeStore;

double getRandomNumber()
{
    return static_cast<double>(rand()) / RAND_MAX;
}

void initializeColony();
void employedBeesPhase();
void onlookerBeesPhase();
void scoutBeesPhase();
void calculateFitness(const vector<int> &tsm);
void printColony();

int main()
{
    srand(time(NULL));
    vector<int> tsm = {3, 1, 2, 3, 0}; // Assuming this is your tsm vector

    Bee myBee;
    myBee.individual = {0, 0, 1, 1, 1, 2, 2, 3};
    myBee.toolLife = {150, 150, 100, 100, 100, 150, 150, 300};
    totalTools = myBee.individual.size();

    for (int i = 0; i < totalTools; i++)
    {
        toolLifeStore[myBee.individual[i]] = myBee.toolLife[i];
    }

    fill(colony.begin(), colony.end(), myBee);
    initializeColony();
    calculateFitness(tsm);

    for (int generation = 0; generation < maxGenerations; generation++)
    {
        employedBeesPhase();
        onlookerBeesPhase();
        scoutBeesPhase();

        calculateFitness(tsm);

        cout << "Generation " << generation + 1 << ":\n";
        printColony();
    }

    return 0;
}

void initializeColony()
{
    for (int i = 0; i < totalBees; i++)
    {
        for (int j = 0; j < totalTools; j++)
        {
            int randomIndex1 = rand() % totalTools;
            int randomIndex2 = rand() % totalTools;
            swap(colony[i].individual[randomIndex1], colony[i].individual[randomIndex2]);
            swap(colony[i].toolLife[randomIndex1], colony[i].toolLife[randomIndex2]);
        }
    }
}

int getElementIndex(const vector<int> &v, vector<int> &l, int elm, int currIndex)
{
    int index = 0;
    int ans = INT_MAX;
    int n = v.size();
    vector<int> res;
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == elm && l[i] > 0)
        {
            res.push_back(i);
        }
    }
    if (res.size() == 0)
    {
        cout << "Tool life not sufficient for tool " << elm << endl;
        return -1;
    }
    for (int i = 0; i < res.size(); i++)
    {
        int t1, t2;
        t1 = abs(currIndex - res[i]);
        if (currIndex > res[i])
        {
            t2 = abs(n + res[i] - currIndex);
        }
        else
        {
            t2 = abs(n - res[i] + currIndex);
        }
        int temp = min(t1, t2);
        if (ans > temp)
        {
            ans = temp;
            index = res[i];
        }
    }
    return index;
}

void employedBeesPhase()
{
    for (int i = 0; i < totalBees; i++)
    {
        Bee currentBee = colony[i];
        Bee neighborBee = colony[rand() % totalBees];

        int randomIndex = rand() % totalTools;
        int elementToSwap = currentBee.individual[randomIndex];

        vector<int> tempIndividual = currentBee.individual;
        tempIndividual[randomIndex] = neighborBee.individual[randomIndex];
        int neighborIndex = getElementIndex(tempIndividual, currentBee.toolLife, elementToSwap, randomIndex);

        if (neighborIndex != -1)
        {
            currentBee.individual[randomIndex] = neighborBee.individual[randomIndex];
            currentBee.toolLife[neighborIndex]--;
            currentBee.fitness = 0; // Reset fitness for recalculation in calculateFitness
        }

        if (currentBee.fitness < colony[i].fitness)
        {
            colony[i] = currentBee;
        }
    }
}

void onlookerBeesPhase()
{
    double totalFitness = 0.0;

    for (const Bee &bee : colony)
    {
        totalFitness += 1.0 / (1.0 + bee.fitness);
    }

    for (int i = 0; i < totalBees; i++)
    {
        double randomProbability = getRandomNumber();
        double cumulativeProbability = 0.0;

        for (const Bee &bee : colony)
        {
            cumulativeProbability += 1.0 / (1.0 + bee.fitness) / totalFitness;

            if (randomProbability <= cumulativeProbability)
            {
                Bee currentBee = colony[i];
                Bee neighborBee = bee;

                int randomIndex = rand() % totalTools;
                int elementToSwap = currentBee.individual[randomIndex];

                vector<int> tempIndividual = currentBee.individual;
                tempIndividual[randomIndex] = neighborBee.individual[randomIndex];
                int neighborIndex = getElementIndex(tempIndividual, currentBee.toolLife, elementToSwap, randomIndex);

                if (neighborIndex != -1)
                {
                    currentBee.individual[randomIndex] = neighborBee.individual[randomIndex];
                    currentBee.toolLife[neighborIndex]--;
                    currentBee.fitness = 0; // Reset fitness for recalculation in calculateFitness
                }

                if (currentBee.fitness < colony[i].fitness)
                {
                    colony[i] = currentBee;
                }

                break;
            }
        }
    }
}

void scoutBeesPhase()
{
    for (int i = 0; i < totalBees; i++)
    {
        if (getRandomNumber() < 0.1)
        { // Probability of abandoning a solution (adjust as needed)
            for (int j = 0; j < totalTools; j++)
            {
                int randomIndex1 = rand() % totalTools;
                int randomIndex2 = rand() % totalTools;
                swap(colony[i].individual[randomIndex1], colony[i].individual[randomIndex2]);
                swap(colony[i].toolLife[randomIndex1], colony[i].toolLife[randomIndex2]);
            }
            colony[i].fitness = 0; // Reset fitness for recalculation in calculateFitness
        }
    }
}

void calculateFitness(const vector<int> &tsm)
{
    for (int i = 0; i < totalBees; ++i)
    {
        if (colony[i].fitness == 0)
        {
            int currIndex = 0;
            int cost = 0;
            const vector<int> &v = colony[i].individual;
            vector<int> &l = colony[i].toolLife;
            set<int> usedTool;
            for (int j = 0; j < productQuantity; j++)
            {
                for (int tool : tsm)
                {
                    int t1, t2;
                    int n = v.size();
                    int toolIndex = getElementIndex(v, l, tool, currIndex);
                    usedTool.insert(toolIndex);
                    if (toolIndex == -1)
                    {
                        cout << "Number of crankshafts made = " << j + 1 << endl;
                        exit(0);
                    }
                    t1 = abs(currIndex - toolIndex);
                    if (currIndex > toolIndex)
                    {
                        t2 = abs(n - currIndex + toolIndex);
                    }
                    else
                    {
                        t2 = abs(n + currIndex - toolIndex);
                    }
                    cost += min(t1, t2);
                    currIndex = toolIndex;
                }
                for (int toolIndex : usedTool)
                {
                    if (toolIndex != -1)
                    {
                        l[toolIndex]--;
                    }
                }
                usedTool.clear();
            }
            colony[i].toolLife = l;
            colony[i].fitness = cost;
        }
    }

    bestBee = *min_element(colony.begin(), colony.end(), [](const Bee &a, const Bee &b)
                           { return a.fitness < b.fitness; });
}

void printColony()
{
    for (int i = 0; i < totalBees; i++)
    {
        cout << "Bee " << i + 1 << ": ";
        cout << "Fitness: " << colony[i].fitness << ", ";
        cout << "Individual: ";
        for (int j = 0; j < totalTools; j++)
        {
            cout << colony[i].individual[j] << " ";
        }
        cout << ", ToolLife: ";
        for (int j = 0; j < totalTools; j++)
        {
            cout << colony[i].toolLife[j] << " ";
        }
        cout << endl;
    }

    cout << "Best Bee: ";
    cout << "Fitness: " << bestBee.fitness << ", ";
    cout << "Individual: ";
    for (int j = 0; j < totalTools; j++)
    {
        cout << bestBee.individual[j] << " ";
    }
    cout << ", ToolLife: ";
    for (int j = 0; j < totalTools; j++)
    {
        cout << bestBee.toolLife[j] << " ";
    }
    cout << endl
         << endl;
}
