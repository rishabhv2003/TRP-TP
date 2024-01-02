#include <bits/stdc++.h>
#include <ctime>

using namespace std;

// global variables
int totalPopulation = 200;
int totalBestPopulation = 100;
double mutationRate = 0.1;
int maxGenerations = 100;

struct Chromosome
{
    int fitnessValue;
    int individual[10];
};

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
void intialPopulationGen(Chromosome population[], int totalTools)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, totalTools - 1);
    for (int i = 0; i < totalPopulation; i++)
    {
        for (int j = 0; j < totalTools; j++)
        {
            population[i].individual[j] = j;
        }
    }
    for (int i = 0; i < totalPopulation; i++)
    {
        for (int j = 0; j < totalTools; j++)
        {
            int m = dis(gen);
            int n = dis(gen);
            swap(population[i].individual[m], population[i].individual[n]);
        }
    }
}
// new function to generate the population.
void initializePopulation(Chromosome population[], int totalTools)
{
    for (int i = 0; i < totalPopulation; i++)
    {
        // Generate a unique individual
        for (int j = 0; j < totalTools; j++)
        {
            population[i].individual[j] = j;
        }
        // Shuffle the individual to make it unique
        std::shuffle(population[i].individual, population[i].individual + totalTools, mt19937(random_device()()));
    }
}

// function to print the population in a file named pop_file.txt
void printIndividualsToFile(Chromosome population[], int populationSize, int totalTools)
{
    ofstream outputFile("pop_size.txt");
    if (outputFile.is_open())
    {
        for (int i = 0; i < populationSize; i++)
        {
            for (int j = 0; j < totalTools; j++)
            {
                outputFile << population[i].individual[j] << " ";
            }
            outputFile << "| " << population[i].fitnessValue;
            outputFile << endl;
        }
        outputFile.close();
    }
    else
    {
        cerr << "Failed to open the file for writing." << std::endl;
    }
}

// function to print the polation.
void printPopulation(Chromosome population[], int populationSize, int totalTools)
{
    for (int i = 0; i < populationSize; i++)
    {
        cout << "Individual " << i << ": ";
        for (int j = 0; j < totalTools; j++)
        {
            cout << population[i].individual[j] << " ";
        }
        cout << "| Fitness: " << population[i].fitnessValue << endl;
    }
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
void calcFitness(Chromosome population[], vector<vector<int>> &jsm, int totalPopulation)
{
    for (int i = 0; i < totalPopulation; i++)
    {
        int currIndex = 0;
        int cost = 0;
        vector<int> v;
        for (int j = 0; j < 10; j++)
        {
            v.push_back(population[i].individual[j]);
        }
        for (int k = 0; k < jsm.size(); k++)
        {
            for (int l = 0; l < jsm[k].size(); l++)
            {
                int toolIndex = getElemenetIndex(v, jsm[k][l]);
                cost += abs(currIndex - toolIndex);
                currIndex = toolIndex;
            }
        }
        population[i].fitnessValue = cost;
    }
}

void selection(Chromosome population[], Chromosome matingPool[], int totalPopulation)
{
    // Sort the population by fitness in ascending order
    sort(population, population + totalPopulation, [](const Chromosome &a, const Chromosome &b) { return a.fitnessValue < b.fitnessValue; });
    // Calculate the total number of ranks
    int totalRanks = totalPopulation * (totalPopulation + 1) / 2;
    // Assign selection probability based on rank
    for (int i = 0; i < totalPopulation; i++)
    {
        double selectionProbability = (double)(i + 1) / totalRanks;
        int selected = 0;
        while (selected < totalPopulation && matingPool[selected].fitnessValue < population[i].fitnessValue)
        {
            selected++;
        }
        matingPool[selected] = population[i];
    }
}

void crossover(Chromosome parent1, Chromosome parent2, Chromosome &child1, Chromosome &child2)
{
    int crossoverPoint = rand() % 10;
    for (int i = 0; i < 10; i++)
    {
        if (i < crossoverPoint)
        {
            child1.individual[i] = parent1.individual[i];
            child2.individual[i] = parent2.individual[i];
        }
        else
        {
            child1.individual[i] = parent2.individual[i];
            child2.individual[i] = parent1.individual[i];
        }
    }
}

void mutate(Chromosome &individual)
{
    if (rand() < mutationRate * RAND_MAX)
    {
        int index1 = rand() % 10;
        int index2 = rand() % 10;
        swap(individual.individual[index1], individual.individual[index2]);
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
    // generaing a population of 100 individuals.
    Chromosome population[totalPopulation];
    // making tool frequency matrix
    toolFreqMat(toolFreq, jsm);
    totalTools = toolFreq.size();
    intialPopulationGen(population, totalTools);
    initializePopulation(population, totalTools);
    int generation = 0;
    while (generation < maxGenerations)
    {
        // Calculate fitness for the current population
        calcFitness(population, jsm, totalPopulation);
        // Create a mating pool through selection
        Chromosome matingPool[totalPopulation];
        selection(population, matingPool, totalPopulation);
        // Create the next generation through crossover and mutation
        for (int i = 0; i < totalPopulation; i += 2)
        {
            int parent1Index = i;
            int parent2Index = i + 1;
            Chromosome child1, child2;
            // Perform crossover
            crossover(population[parent1Index], population[parent2Index], child1, child2);
            // Perform mutation
            if (rand() < mutationRate * RAND_MAX)
            {
                mutate(child1);
            }
            if (rand() < mutationRate * RAND_MAX)
            {
                mutate(child2);
            }
            // Replace parents with children in the population
            population[parent1Index] = child1;
            population[parent2Index] = child2;
        }
        cout << "Generation " << generation << ":" << endl;
        printPopulation(population, totalPopulation, totalTools);
        generation++;
    }
    printIndividualsToFile(population, totalPopulation, totalTools);
    return 0;
}
