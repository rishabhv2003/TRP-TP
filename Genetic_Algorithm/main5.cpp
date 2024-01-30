#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <cstdlib>
#include <time.h>
using namespace std;

class Chromosome
{
public:
	int fitness;
	vector<int> individual;
	bool operator==(const Chromosome &other) const
	{
		return (fitness == other.fitness) && (individual == other.individual);
	}
};

const int totalPopulation = 50;
const int maxGenerations = 100;
int totalTools = 0;
vector<Chromosome> population(totalPopulation);
vector<Chromosome> auxPopulation(totalPopulation);
Chromosome bestIndividual;
unordered_map<int, int> toolFreqMatrix;

void calculateToolFrequency(const vector<vector<int>> &tsm);
void printPopulation();
void printToolFrequencyMatrix();
void initialPopulationGen();
void calculateFitness(const vector<vector<int>> &tsm);
void selection();
void crossover();
void mutation();

int main()
{
	vector<int> toolBatch = {20, 30, 20, 15};
	vector<vector<int>> tsm = {
		{0, 0, 4, 3},
		{1, 8, 6, 2},
		{2, 2, 8, 8},
		{8, 9, 7, 5},
	};

	calculateToolFrequency(tsm);
	totalTools = toolFreqMatrix.size();

	initialPopulationGen();
	calculateFitness(tsm);
	printPopulation();

	for (int i = 0; i < maxGenerations; ++i)
	{
		selection();
		crossover();
		mutation();
		calculateFitness(tsm);
		cout << "Population after " << i + 1 << "th iteration" << endl;
		printPopulation();
	}
	return 0;
}

void calculateToolFrequency(const vector<vector<int>> &tsm)
{
	for (const auto &row : tsm)
	{
		for (int tool : row)
		{
			toolFreqMatrix[tool]++;
		}
	}
}

void printPopulation()
{
	for (int i = 0; i < totalPopulation; ++i)
	{
		cout << "Individual " << i << ": ";
		for (int tool : population[i].individual)
		{
			cout << tool << " ";
		}
		cout << "| Fitness: " << population[i].fitness << endl;
	}
}

void initialPopulationGen()
{
	srand(time(nullptr));
	for (int i = 0; i < totalPopulation; ++i)
	{
		population[i].individual.resize(totalTools);
		iota(population[i].individual.begin(), population[i].individual.end(), 0);
		random_shuffle(population[i].individual.begin(), population[i].individual.end());
	}
}

int getElementIndex(const vector<int> &v, int elm)
{
	auto it = find(v.begin(), v.end(), elm);
	if (it != v.end())
	{
		return distance(v.begin(), it);
	}
	return -1;
}

void calculateFitness(const vector<vector<int>> &tsm)
{
	for (int i = 0; i < totalPopulation; ++i)
	{
		int currIndex = 0;
		int cost = 0;
		vector<int> v(population[i].individual.begin(), population[i].individual.begin() + 10);
		for (const auto &row : tsm)
		{
			for (int tool : row)
			{
				int t1, t2;
				int toolIndex = getElementIndex(v, tool);
				t1 = abs(currIndex - toolIndex);
				if (currIndex > toolIndex)
				{
					t2 = abs(static_cast<int>(v.size()) - currIndex + toolIndex);
				}
				else
				{
					t2 = abs(static_cast<int>(v.size()) + currIndex - toolIndex);
				}
				cost += min(t1, t2);
				currIndex = toolIndex;
			}
		}
		population[i].fitness = cost;
	}
}

bool compareChromosomes(const Chromosome &a, const Chromosome &b)
{
	return a.fitness < b.fitness;
}
Chromosome findMinFitnessChromosome(const vector<Chromosome> &chromosomes)
{
	return *min_element(chromosomes.begin(), chromosomes.end(), compareChromosomes);
}
void selection()
{
	srand(static_cast<unsigned>(time(0)));
	Chromosome best;
	best = population[0];
	for (int i = 1; i < totalPopulation; i++)
	{
		if (population[i].fitness < best.fitness)
		{
			best = population[i];
		}
	}
	auxPopulation.assign({best});
	for (int i = 1; i < totalPopulation; i += 2)
	{
		Chromosome p1;
		Chromosome p2;
		p1.fitness = 0;
		p2.fitness = 0;
		vector<Chromosome> in(3);
		while (p1 == p2)
		{
			in[0] = population[rand() % totalPopulation];
			in[1] = population[rand() % totalPopulation];
			in[2] = population[rand() % totalPopulation];
			p1 = findMinFitnessChromosome(in);

			in[0] = population[rand() % totalPopulation];
			in[1] = population[rand() % totalPopulation];
			in[2] = population[rand() % totalPopulation];
			p2 = findMinFitnessChromosome(in);
		}
		auxPopulation.push_back(p1);
		auxPopulation.push_back(p2);
	}
	population.assign(auxPopulation.begin(), auxPopulation.end());
	auxPopulation.clear();
}

void crossover()
{
	const double crossover_rate = 0.8;
	for (int i = 1; i < totalPopulation; i += 2)
	{
		if (static_cast<double>(rand()) / RAND_MAX < crossover_rate)
		{
			int crossover_point = rand() % (totalTools - 1) + 1;
			vector<int> &parent1 = population[i].individual;
			vector<int> &parent2 = population[i + 1].individual;
			vector<int> child1(totalTools, -1);
			vector<int> child2(totalTools, -1);

			copy(parent2.begin() + crossover_point, parent2.end(), child1.begin() + crossover_point);
			copy(parent1.begin() + crossover_point, parent1.end(), child2.begin() + crossover_point);

			vector<int> unused_numbers1;
			vector<int> unused_numbers2;
			for (int num : parent1)
			{
				if (find(child1.begin(), child1.end(), num) == child1.end())
				{
					unused_numbers1.push_back(num);
				}
			}
			for (int num : parent2)
			{
				if (find(child2.begin(), child2.end(), num) == child2.end())
				{
					unused_numbers2.push_back(num);
				}
			}
			int index1 = 0, index2 = 0;
			for (int j = 0; j < totalTools; ++j)
			{
				if (child1[j] == -1)
				{
					child1[j] = unused_numbers1[index1++];
				}

				if (child2[j] == -1)
				{
					child2[j] = unused_numbers2[index2++];
				}
			}
			population[i].individual = child1;
			population[i + 1].individual = child2;
		}
	}
}

void mutation()
{
	const double mutation_rate = 0.1;
	for (int i = 1; i < totalPopulation; ++i)
	{
		if (static_cast<double>(rand()) / RAND_MAX < mutation_rate)
		{
			vector<int> mutation_positions(totalTools);
			iota(mutation_positions.begin(), mutation_positions.end(), 0);
			random_shuffle(mutation_positions.begin(), mutation_positions.end());
			mutation_positions.resize(2);
			swap(population[i].individual[mutation_positions[0]], population[i].individual[mutation_positions[1]]);
		}
	}
}
