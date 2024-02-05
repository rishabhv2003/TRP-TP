// Genetic Algorithm for Tool Placement with Tools duplication.

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <ctime>
using namespace std;

class Chromosome
{
public:
    int fitness;
    vector<int> individual;
    vector<int> toolLife;
    bool operator==(const Chromosome &other) const
    {
        return (fitness == other.fitness) && (individual == other.individual);
    }
};

const int totalPopulation = 50;
const int maxGenerations = 100;
int totalTools;
vector<Chromosome> population(totalPopulation);
vector<Chromosome> auxPopulation(totalPopulation);
Chromosome bestIndividual;
unordered_map<int, int> toolLifeStore;

void initialPopulationGen();
void selection();
void crossover();
void mutate();
void regenerateToolLife();
void calculateFitness(const vector<int> &tsm);
void traversePopulation();
void traverseToolLifeStore();

int main()
{
    srand(time(NULL));
    // vector<int> tsm = {2, 4, 0, 5, 2, 5, 1, 3, 2, 0};
    // vector<int> tsm = {0, 1, 2, 3, 4, 5, 2, 5, 6, 2, 7, 8, 5, 8, 9};
    vector<int> tsm = {0, 0, 4, 3, 1, 8, 6, 2, 2, 2, 8, 8, 8, 9, 7, 5};

    Chromosome myChromosome;
    myChromosome.fitness = 100;
    // myChromosome.individual = {0, 2, 3, 4, 5, 2, 3, 1, 2, 1};
    myChromosome.individual = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    // myChromosome.toolLife = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    myChromosome.toolLife = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

    totalTools = myChromosome.individual.size();
    for (int i = 0; i < totalTools; i++)
    {
        toolLifeStore[myChromosome.individual[i]] = myChromosome.toolLife[i];
    }

    fill(population.begin(), population.end(), myChromosome);
    initialPopulationGen();
    calculateFitness(tsm);
    for (int i = 0; i < maxGenerations; ++i)
    {
        // cout << "Before Selection" << endl;
        selection();
        // cout << "After Selection" << endl;

        // cout << "Before Crossover" << endl;
        // crossover();
        // cout << "After Crossover" << endl;

        // cout << "Before Mutation" << endl;
        mutate();
        // cout << "After Mutation" << endl;

        // cout << "Before tool life" << endl;
        regenerateToolLife();
        // cout << "After tool life" << endl;

        // cout << "before calculate fitness" << endl;
        calculateFitness(tsm);
        // cout << "after calculate fitness" << endl;

        cout << "Population after " << i + 1 << "th iteration" << endl;
        traversePopulation();
    }
    return 0;
}
void initialPopulationGen()
{
    for (int i = 0; i < totalPopulation; i++)
    {
        for (int j = 0; j < totalTools; j++)
        {
            int randomIndex1 = rand() % totalTools;
            int randomIndex2 = rand() % totalTools;
            swap(population[i].individual[randomIndex1], population[i].individual[randomIndex2]);
            swap(population[i].toolLife[randomIndex1], population[i].toolLife[randomIndex2]);
        }
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
            int crossover_point = (rand() % (totalTools - 2)) + 1;

            vector<int> &parent1 = population[i].individual;
            vector<int> &parent2 = population[i + 1].individual;
            vector<int> child1, child2;

            child1.assign(parent1.begin(), parent1.begin() + crossover_point);
            child1.insert(child1.end(), parent2.begin() + crossover_point + 1, parent2.end());

            child2.assign(parent2.begin(), parent2.begin() + crossover_point);
            child2.insert(child2.end(), parent1.begin() + crossover_point + 1, parent1.end());

            population[i].individual = child1;
            population[i + 1].individual = child2;
        }
    }
}

void mutate()
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
void regenerateToolLife()
{
    for (int i = 0; i < totalPopulation; i++)
    {
        for (int j = 0; j < totalTools; j++)
        {
            int tool = population[i].individual[j];
            population[i].toolLife[j] = toolLifeStore[tool];
        }
    }
}
void traverseToolLifeStore()
{
    cout << "Tool Life Store:\n";
    for (const auto &entry : toolLifeStore)
    {
        cout << "Tool: " << entry.first << ", Life: " << entry.second << endl;
    }
}
void traversePopulation()
{
    for (int i = 0; i < totalPopulation; i++)
    {
        cout << "Chromosome " << i + 1 << ": ";
        cout << "Fitness: " << population[i].fitness << ", ";
        cout << "Individual: ";
        for (int j = 0; j < totalTools; j++)
        {
            cout << population[i].individual[j] << " ";
        }
        cout << ", ToolLife: ";
        for (int j = 0; j < totalTools; j++)
        {
            cout << population[i].toolLife[j] << " ";
        }
        cout << endl;
    }
}

int getElementIndex(const vector<int> &v, vector<int> &l, int elm, int currIndex)
{
    int index = 0;
    int ans = 100;
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
        exit(0);
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
    l[index]--;
    return index;
}

void calculateFitness(const vector<int> &tsm)
{
    for (int i = 0; i < totalPopulation; ++i)
    {
        int currIndex = 0;
        int cost = 0;
        vector<int> v(population[i].individual.begin(), population[i].individual.begin() + totalTools);
        vector<int> l(population[i].toolLife.begin(), population[i].toolLife.begin() + totalTools);
        for (int tool : tsm)
        {
            int t1, t2;
            int n = v.size();
            int toolIndex = getElementIndex(v, l, tool, currIndex);
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
        population[i].toolLife = l;
        population[i].fitness = cost;
    }
}