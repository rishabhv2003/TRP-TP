import random
import copy

class Chromosome:
    def __init__(self):
        self.fitness = 0
        self.individual = []
        self.toolLife = []

def initial_population_gen():
    for i in range(total_population):
        for j in range(total_tools):
            random_index1 = random.randint(0, total_tools - 1)
            random_index2 = random.randint(0, total_tools - 1)
            population[i].individual[random_index1], population[i].individual[random_index2] = \
                population[i].individual[random_index2], population[i].individual[random_index1]
            population[i].toolLife[random_index1], population[i].toolLife[random_index2] = \
                population[i].toolLife[random_index2], population[i].toolLife[random_index1]

def compare_chromosomes(a, b):
    return a.fitness < b.fitness

def find_min_fitness_chromosome(chromosomes):
    return min(chromosomes, key=lambda x: x.fitness)

def selection():
    best = find_min_fitness_chromosome(population)
    aux_population = [copy.deepcopy(best)]

    for i in range(1, total_population, 2):
        p1, p2 = Chromosome(), Chromosome()
        p1.fitness, p2.fitness = 0, 0
        individuals = random.sample(population, 3)
        p1 = find_min_fitness_chromosome(individuals)
        individuals = random.sample(population, 3)
        p2 = find_min_fitness_chromosome(individuals)

        while p1 == p2:
            individuals = random.sample(population, 3)
            p1 = find_min_fitness_chromosome(individuals)
            individuals = random.sample(population, 3)
            p2 = find_min_fitness_chromosome(individuals)

        aux_population.extend([copy.deepcopy(p1), copy.deepcopy(p2)])

    population.clear()
    population.extend(aux_population)

def crossover():
    crossover_rate = 0.8

    for i in range(1, total_population, 2):
        if random.random() < crossover_rate:
            crossover_point = random.randint(1, total_tools - 2)

            parent1, parent2 = population[i].individual, population[i + 1].individual
            child1 = parent1[:crossover_point] + parent2[crossover_point + 1:]
            child2 = parent2[:crossover_point] + parent1[crossover_point + 1:]

            population[i].individual, population[i + 1].individual = child1, child2

def mutate():
    mutation_rate = 0.1

    for i in range(1, total_population):
        if random.random() < mutation_rate:
            mutation_positions = list(range(total_tools))
            random.shuffle(mutation_positions)
            mutation_positions = mutation_positions[:2]
            population[i].individual[mutation_positions[0]], population[i].individual[mutation_positions[1]] = \
                population[i].individual[mutation_positions[1]], population[i].individual[mutation_positions[0]]

def regenerate_tool_life():
    for i in range(total_population):
        for j in range(total_tools):
            tool = population[i].individual[j]
            population[i].toolLife[j] = tool_life_store[tool]

def traverse_tool_life_store():
    print("Tool Life Store:")
    for tool, life in tool_life_store.items():
        print(f"Tool: {tool}, Life: {life}")


def traverse_population():
    for i, chromosome in enumerate(population):
        print(f"Chromosome {i + 1}: Fitness: {chromosome.fitness}, Individual: {chromosome.individual}, ToolLife: {chromosome.toolLife}")

def get_element_index(v, l, elm, curr_index):
    index = 0
    ans = float('inf')
    n = len(v)

    res = []
    for i in range(n):
        if v[i] == elm and l[i] > 0:
            res.append(i)
    # print(v, elm ,l)
    if len(res) == 0:
        print(v, l)
        print(f"Tool life not sufficient for tool {elm}")
        return -1

    for i in res:
        t1, t2 = abs(curr_index - i), 0
        if curr_index > i:
            t2 = abs(n + i - curr_index)
        else:
            t2 = abs(n - i + curr_index)
        temp = min(t1, t2)
        if ans > temp:
            ans = temp
            index = i

    return index

def calculate_fitness(tsm):
    for i in range(total_population):
        curr_index = 0
        cost = 0
        v = population[i].individual
        l = population[i].toolLife
        used_tools = set()

        for j in range(product_quantity):
            for tool in tsm:
                n = len(v)
                tool_index = get_element_index(v, l, tool, curr_index)
                used_tools.add(tool_index)

                if tool_index == -1:
                    print(f"Number of crankshafts made = {j}")
                    return

                t1, t2 = abs(curr_index - tool_index), 0
                if curr_index > tool_index:
                    t2 = abs(n - curr_index + tool_index)
                else:
                    t2 = abs(n + curr_index - tool_index)
                cost += min(t1, t2)
                curr_index = tool_index

            for tool_index in used_tools:
                if tool_index != -1:
                    l[tool_index] -= 1
            used_tools.clear()

        population[i].toolLife = l
        population[i].fitness = cost


product_quantity = 30
total_population = 10
max_generations = 5
total_tools = 0
population = [Chromosome() for _ in range(total_population)]
aux_population = [Chromosome() for _ in range(total_population)]
best_individual = Chromosome()
tool_life_store = {}

if __name__ == "__main__":
    random.seed()
    tsm = [3, 1, 2, 3, 0]
    my_chromosome = Chromosome()
    my_chromosome.individual = [0, 0, 1, 1, 1, 2, 2, 3]
    my_chromosome.toolLife = [15, 15, 10, 10, 10, 15, 15, 30]
    total_tools = len(my_chromosome.individual)

    for i in range(total_tools):
        tool_life_store[my_chromosome.individual[i]] = my_chromosome.toolLife[i]

    population = [copy.deepcopy(my_chromosome) for _ in range(total_population)]
    initial_population_gen()
    traverse_population()
    calculate_fitness(tsm)
    traverse_population()
    # for i in range(max_generations):
    #     selection()
    #     crossover()
    #     mutate()
    #     regenerate_tool_life()
    #     calculate_fitness(tsm)

    #     print(f"Population after {i + 1}th iteration")
    #     traverse_population()
