#include <bits/stdc++.h>
#include <chrono>
#include <random>
using namespace std;

struct Chromosome
{
	float fitness;
	vector<int> individual;
};

// global variables
const int totalPopulation = 100;
const int maxGenerations = 100;
int totalTools;
vector<Chromosome> population(totalPopulation);
vector<Chromosome> prevPopulation(totalPopulation);
map<int, int> toolFreqMatrix;

// functions
void calculateToolFrequency(vector<vector<int>> tsm)
{
	for (int i = 0; i < tsm.size(); i++)
	{
		for (int j = 0; j < tsm[i].size(); j++)
		{
			toolFreqMatrix[tsm[i][j]]++;
		}
	}
}

void printPopulation()
{
	for (int i = 0; i < totalPopulation; i++)
	{
		cout << "Individual " << i << ": ";
		for (int j = 0; j < totalTools; j++)
		{
			cout << population[i].individual[j] << " ";
		}
		cout << "| Fitness: " << population[i].fitness << endl;
	}
}

void printToolFrequencyMatrix()
{
	cout << "Tool Frequency Matrix:" << endl;
	for (const auto &entry : toolFreqMatrix)
	{
		cout << "Tool " << entry.first << ": " << entry.second << " times" << endl;
	}
}

void intialPopulationGen()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);
	std::uniform_int_distribution<> dis(0, totalTools - 1);
	for (int i = 0; i < totalPopulation; i++)
	{
		for (int j = 0; j < totalTools; j++)
		{
			population[i].individual.push_back(j);
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

int getElemenetIndex(const vector<int> v, int elm)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (elm == v[i])
            return i;
    }
    return -1;
}
void calculateFitness(vector<vector<int>> &tsm)
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
        for (int k = 0; k < tsm.size(); k++)
        {
            for (int l = 0; l < tsm[k].size(); l++)
            {
                int toolIndex = getElemenetIndex(v, tsm[k][l]);
                int t1 = abs(currIndex - toolIndex);
                int t2 = abs(v.size() - currIndex + toolIndex);
                cost += min(t1 , t2);
                currIndex = toolIndex;
            }
        }
        population[i].fitness = cost;
    }
}

void selection()
{
	
}

void crossover()
{
	
}

void mutate()
{
	
}

int main()
{
	// we can implement input for the tools and the tool sequence matrix.

	vector<int> toolBatch = {20, 30, 20, 15};
	vector<vector<int>> tsm = {
		{6, 4, 0, 2, 9},
		{3, 6, 9, 8, 7},
		{6, 0, 1, 5, 4},
		{1, 2, 0, 4, 3},
	};

	calculateToolFrequency(tsm);
	totalTools = toolFreqMatrix.size();

	intialPopulationGen();
	calculateFitness(tsm);
	// printPopulation();
	// printToolFrequencyMatrix();
	int generation = 0;

	while(generation < maxGenerations)
	{
		// selection
		// cross over
		// mutation
	}

	return 0;
}
