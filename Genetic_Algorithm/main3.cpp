// Genetic Algorithm for Tool Placement with Tools duplication.
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <ctime>
#include <set>
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
const int maxGenerations = 2;
const int productQuantity = 300;
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
    // vector<int> tsm = {0, 1, 5, 4, 6, 4, 5, 6, 3, 2};
    vector<int> tsm = {3, 1, 2, 3, 0};
    Chromosome myChromosome;

    // myChromosome.individual = {
    //     0, 0, 0, 0, 0, 0, 0, 0, 0,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     2, 2, 2, 2,
    //     3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    //     4, 4, 4, 4,
    //     5, 5, 5, 5,
    //     6, 6, 6, 6};
    myChromosome.individual = {0, 0, 1, 1, 1, 2, 2, 3};
    // myChromosome.toolLife = {
    //     154, 154, 154, 154, 154, 154, 154, 154, 154,
    //     131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
    //     326, 326, 326, 326,
    //     326, 326, 326, 326, 326, 326, 326, 326, 326, 326,
    //     326, 326, 326, 326,
    //     326, 326, 326, 326,
    //     326, 326, 326, 326};
    myChromosome.toolLife = {150, 150, 100, 100, 100, 150, 150, 300};
    totalTools = myChromosome.individual.size();
    // totalTools = 45;
    cout << "Total Tools " << totalTools << endl;
    for (int i = 0; i < totalTools; i++)
    {
        toolLifeStore[myChromosome.individual[i]] = myChromosome.toolLife[i];
    }

    fill(population.begin(), population.end(), myChromosome);
    initialPopulationGen();
    calculateFitness(tsm);
    for (int i = 0; i < maxGenerations; ++i)
    {
        selection();
        // crossover();
        mutate();
        regenerateToolLife();
        calculateFitness(tsm);
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

void calculateFitness(const vector<int> &tsm)
{
    for (int i = 0; i < totalPopulation; ++i)
    {
        int currIndex = 0;
        int cost = 0;
        const vector<int> &v = population[i].individual;
        vector<int> &l = population[i].toolLife;
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
        population[i].toolLife = l;
        population[i].fitness = cost;
    }
}