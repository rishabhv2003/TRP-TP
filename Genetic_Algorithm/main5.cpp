#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <cstdlib>
#include <time.h>
using namespace std;

class Chromosome {
public:
	int fitness;
	vector<int> individual;
	bool operator==(const Chromosome& other) const {
		return (fitness == other.fitness) && (individual == other.individual);
	}
};

// Global variables
const int totalPopulation = 50;
const int maxGenerations = 100;
int totalTools = 0;
vector<Chromosome> population(totalPopulation);
vector<Chromosome> auxPopulation(totalPopulation);
Chromosome bestIndividual;
unordered_map<int, int> toolFreqMatrix;

// Function prototypes
void calculateToolFrequency(const vector<vector<int>>& tsm);
void printPopulation();
void printToolFrequencyMatrix();
void initialPopulationGen();
int getElementIndex(const vector<int>& v, int elm);
void calculateFitness(const vector<vector<int>>& tsm);
void selection();
void crossover();
void mutate();

int main() {
	// We can implement input for the tools and the tool sequence matrix.
	vector<int> toolBatch = {20, 30, 20, 15};
	// the tool sequence matrix contains the sequence of tools required for
	// operating on each batch.
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

	for (int i = 0; i < maxGenerations; ++i) {
		// cout << "Before Selection" << endl;
		selection();
		// cout << "After Selection" << endl;

		// cout << "Before Crossover" << endl;
		crossover();
		// cout << "After Crossover" << endl;

		// cout << "Before Mutation" << endl;
		mutate();
		// cout << "After Mutation" << endl;

		// cout << "before calculate fitness" << endl;
		calculateFitness(tsm);
		// cout << "after calculate fitness" << endl;

		cout << "Population after " << i + 1 << "th iteration" << endl;

		printPopulation();
	}
	return 0;
}


// This function stores the frequency of each tool in the toolFreqMatrix (map).
void calculateToolFrequency(const vector<vector<int>>& tsm) {
	for (const auto& row : tsm) {
		for (int tool : row) {
			toolFreqMatrix[tool]++;
		}
	}
}

void printPopulation() {
	for (int i = 0; i < totalPopulation; ++i) {
		cout << "Individual " << i << ": ";
		for (int tool : population[i].individual) {
			cout << tool << " ";
		}
		cout << "| Fitness: " << population[i].fitness << endl;
	}
}

// populates the "population" on the basis of number of tool.
void initialPopulationGen() {
	srand(time(nullptr));
	for (int i = 0; i < totalPopulation; ++i) {
		population[i].individual.resize(totalTools);
		iota(population[i].individual.begin(), population[i].individual.end(), 0);
		random_shuffle(population[i].individual.begin(), population[i].individual.end());
	}
}

// This function return the index of elm from v.
int getElementIndex(const vector<int>& v, int elm) {
	auto it = find(v.begin(), v.end(), elm);
	if (it != v.end()) {
		return distance(v.begin(), it);
	}
	return -1;
}

void calculateFitness(const vector<vector<int>>& tsm) {
	for (int i = 0; i < totalPopulation; ++i) {
		int currIndex = 0;
		int cost = 0;
		// storing the [9,1,2..] tool sequence of population in vector v.
		vector<int> v(population[i].individual.begin(), population[i].individual.begin() + 10);
		// traversing tsm and calling the tools from v just like magazine is moving clockwise and anticlockwise.
		for (const auto& row : tsm) {
			for (int tool : row) {
				int toolIndex = getElementIndex(v, tool);
				int t1 = abs(currIndex - toolIndex);
				int t2 = abs(static_cast<int>(v.size()) - currIndex + toolIndex);
				cost += min(t1, t2);
				currIndex = toolIndex;
			}
		}
		population[i].fitness = cost;
	}
}

bool compareChromosomes(const Chromosome &a, const Chromosome &b) {
	return a.fitness < b.fitness;
}
Chromosome findMinFitnessChromosome(const vector<Chromosome> &chromosomes) {
	return *min_element(chromosomes.begin(), chromosomes.end(), compareChromosomes);
}
void selection() {
	srand(static_cast<unsigned>(time(0)));
	Chromosome best1;
	Chromosome best2;
	best1 = population[0];
	for (int i = 1; i < totalPopulation; i++) {
		if (population[i].fitness < best1.fitness) {
			best1 = population[i];
		}
	}
	auxPopulation.assign({best1});

	for (int i = 1; i < totalPopulation; i += 2) {
		Chromosome p1;
		Chromosome p2;
		p1.fitness = 0;
		p2.fitness = 0;
		vector<Chromosome> in(3);
		while (p1 == p2) {
			in[0] = population[rand() % totalPopulation];
			in[1] = population[rand() % totalPopulation];
			in[2] = population[rand() % totalPopulation];
			p1 = findMinFitnessChromosome(in);
			// cout << "p1 fitness " << p1.fitness << endl;
			in[0] = population[rand() % totalPopulation];
			in[1] = population[rand() % totalPopulation];
			in[2] = population[rand() % totalPopulation];
			p2 = findMinFitnessChromosome(in);
			// cout << "p2 fitness " << p2.fitness << endl;
		}
		auxPopulation.push_back(p1);
		auxPopulation.push_back(p2);
	}
	population.assign(auxPopulation.begin(), auxPopulation.end());
	auxPopulation.clear();
}


void crossover() {
	const double crossover_rate = 0.8;
	for (int i = 1; i < totalPopulation; i += 2) {
		// Randomly decide whether to perform crossover
		if (static_cast<double>(rand()) / RAND_MAX < crossover_rate) {
			// Select a random crossover point
			int crossover_point = rand() % (totalTools - 1) + 1;
			// Perform one-point crossover
			vector<int>& parent1 = population[i].individual;
			vector<int>& parent2 = population[i + 1].individual;
			vector<int> child1(totalTools, -1);
			vector<int> child2(totalTools, -1);
			// Fill the crossover segment
			copy(parent2.begin() + crossover_point, parent2.end(), child1.begin() + crossover_point);
			copy(parent1.begin() + crossover_point, parent1.end(), child2.begin() + crossover_point);
			// Fill the remaining positions with unused numbers
			vector<int> unused_numbers1;
			vector<int> unused_numbers2;
			for (int num : parent1) {
				if (find(child1.begin(), child1.end(), num) == child1.end()) {
					unused_numbers1.push_back(num);
				}
			}
			for (int num : parent2) {
				if (find(child2.begin(), child2.end(), num) == child2.end()) {
					unused_numbers2.push_back(num);
				}
			}
			int index1 = 0, index2 = 0;
			for (int j = 0; j < totalTools; ++j) {
				if (child1[j] == -1) {
					child1[j] = unused_numbers1[index1++];
				}

				if (child2[j] == -1) {
					child2[j] = unused_numbers2[index2++];
				}
			}
			// Update the population with the new individuals
			population[i].individual = child1;
			population[i + 1].individual = child2;
		}
	}
}

void mutate() {
	const double mutation_rate = 0.1;

	for (int i = 1; i < totalPopulation; ++i) {
		// Randomly decide whether to perform mutation
		if (static_cast<double>(rand()) / RAND_MAX < mutation_rate) {
			// Select two distinct positions for mutation
			vector<int> mutation_positions(totalTools);
			iota(mutation_positions.begin(), mutation_positions.end(), 0);
			random_shuffle(mutation_positions.begin(), mutation_positions.end());
			mutation_positions.resize(2);

			// Perform swap mutation
			swap(population[i].individual[mutation_positions[0]], population[i].individual[mutation_positions[1]]);
		}
	}
}
