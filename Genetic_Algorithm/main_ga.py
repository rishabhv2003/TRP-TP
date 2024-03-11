import random

class Chromosome:
    def __init__(self):
        self.fitness = 0
        self.individual = []
        self.tool_life = []

total_population = 1000
max_generations = 500
product_quantity = 1304
total_tools = 0
population = [Chromosome() for _ in range(total_population)]
aux_population = [Chromosome() for _ in range(total_population)]
best_individual = Chromosome()
tool_life_store = {}


def initial_population_gen():
    global population, total_tools
    for i in range(total_population):
        for _ in range(total_tools):
            random_index_1, random_index_2 = random.sample(range(total_tools), 2)
            population[i].individual[random_index_1], population[i].individual[random_index_2] = \
                population[i].individual[random_index_2], population[i].individual[random_index_1]

            population[i].tool_life[random_index_1], population[i].tool_life[random_index_2] = \
                population[i].tool_life[random_index_2], population[i].tool_life[random_index_1]

def find_min_fitness_chromosome(chromosomes):
    return min(chromosomes, key=lambda x: x.fitness)

def selection():
    global aux_population, population
    best = population[0]
    for i in range(1, total_population):
        if population[i].fitness < best.fitness:
            best = population[i]
            
    aux_population = [best]
    for i in range(1, total_population, 2):
        p1, p2 = population[0], population[0]
        p1.fitness, p2.fitness = 0, 0
        in_list = [Chromosome() for _ in range(3)]
        while p1 == p2:
            in_list[0], in_list[1], in_list[2] = random.choice(population), random.choice(population), random.choice(population)
            p1 = find_min_fitness_chromosome(in_list)
            in_list[0], in_list[1], in_list[2] = random.choice(population), random.choice(population), random.choice(population)
            p2 = find_min_fitness_chromosome(in_list)
        aux_population.extend([p1, p2])
        
    population[:] = aux_population[:]
    aux_population = []

def crossover():
    crossover_rate = 0.8
    for i in range(1, total_population, 2):
        if random.random() < crossover_rate:
            crossover_point = random.randint(1, total_tools - 2)

            parent1, parent2 = population[i].individual, population[i + 1].individual
            child1 = parent1[:crossover_point] + parent2[crossover_point + 1:]
            child2 = parent2[:crossover_point] + parent1[crossover_point + 1:]

            population[i].individual = child1
            population[i + 1].individual = child2

def mutate():
    mutation_rate = 0.5
    for i in range(1, total_population):
        if random.random() < mutation_rate:
            pos1, pos2 = random.sample(range(total_tools), 2)
            population[i].individual[pos1], population[i].individual[pos2] = \
                population[i].individual[pos2], population[i].individual[pos1]

def regenerate_tool_life():
    for i in range(total_population):
        for j in range(total_tools):
            tool = population[i].individual[j]
            population[i].tool_life[j] = tool_life_store[tool]

def traverse_tool_life_store():
    print("Tool Life Store:")
    for tool, life in tool_life_store.items():
        print(f"Tool: {tool}, Life: {life}")

def traverse_population():
    for i in range(total_population):
        print(f"Chromosome {i + 1}: Fitness: {population[i].fitness}, Individual: {population[i].individual}, ToolLife: {population[i].tool_life}")

def get_element_index(arr, elm, curr_index):
    index = 0
    ans = float('inf')
    n = len(arr)

    res = []
    for i in range(n):
        if arr[i][1] == elm and arr[i][2] > 0:
            res.append(arr[i][0])
    # print("Elm Arr",arr)
    # print(res)
    if len(res) == 0:
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
        
def find_greedy_sol(arr, tsm):
    sol = []
    used = set()
    curr = 0
    for tool in tsm:
        if tool not in used:        
            idx = get_element_index(arr, tool, curr)
            used.add(tool)
            if idx == -1:
                return -1
        else:
            # print("Yeh Sol hai: ", sol)
            for s in sol:
                # print(s)
                if s[1] == tool:
                    idx = s[0]

        sol.append(arr[idx])
        curr = idx
    return sol
    
def calculate_fitness(arr, tsm, jobs):
    fitness = 0
    while jobs > 0:
      
        sol = find_greedy_sol(arr, tsm)
        if sol == -1:
            print("Hogai Bhasad!!")
            return 0
        # print("Greedy Solution: ", sol)
        indexes = list(map(lambda x: x[0], sol))
        min_tl = min(list(map(lambda x: x[2], sol)))
        # print("min tool life: ", min_tl)
        set_of_indexes = set(indexes)
        for i in set_of_indexes:
            arr[i][2]-=min_tl

        # print("Array after reducing tool lifes: ", arr)
        # print("Selected indexes", indexes)
        # print("Set of indexes", set_of_indexes)
        cost = 0
        curr = indexes[0]
        for req in indexes[1:]:
            t1, t2 = abs(req - curr), 0
            # print(f"curr {curr}, req: {req}")
            if req > curr:
                t2 = abs(len(arr) + curr - req)
            else:
                t2 = abs(len(arr) - curr + req)
            cost += min(t1, t2)
            # print(f"t1: {t1}, t2: {t2}, cost: {cost}")
            curr = req

        # print("Cost is: ", cost)
        fitness += ((cost)*min_tl)
        # print("Fitness at this step: ", fitness)
        jobs-=min_tl
        
    return fitness

def packer(ind, life):
    arr = []
    for i, a in enumerate(zip(ind, life)):
        arr.append([i, a[0], a[1]])
        
    return arr
        
if __name__ == "__main__":
    random.seed()
    tsm = [0, 1, 5, 4, 6, 4, 5, 6, 3, 2]
    my_chromosome = Chromosome()
    my_chromosome.individual = \
        [3, 6, 4, 5, 1,
       0, 1, 3, 0, 1,
       0, 1, 5, 4, 6, 
       3, 3, 2, 0, 1,
       0, 1, 0, 1, 5,
       4, 6, 3, 2, 3,
       3, 2, 3, 3, 6,
       4, 5, 1, 0, 1,
       0, 1, 0, 3, 2]
    my_chromosome.tool_life = \
        [326, 326, 326, 326, 131,
      154, 131, 326, 154, 131,
      154, 131, 326, 326, 326,
      326, 326, 326, 154, 131,
      154, 131, 154, 131, 326,
      326, 326, 326, 326, 326,
      326, 326, 326, 326, 326,
      326, 326, 131, 154, 131,
      154, 131, 154, 326, 326]
    total_tools = len(my_chromosome.individual)

    for i in range(total_tools):
        tool_life_store[my_chromosome.individual[i]] = my_chromosome.tool_life[i]

    population = [Chromosome() for _ in range(total_population)]    
    for chromosome in population:
        chromosome.individual = my_chromosome.individual[:]
        chromosome.tool_life = my_chromosome.tool_life[:]
    
    # traverse_tool_life_store()
    initial_population_gen()
    
    for i in range(total_population):
        arr = packer(population[i].individual[:], population[i].tool_life[:])
        # print("Internal Arr", arr)
        fitness = calculate_fitness(arr, tsm, product_quantity)
        population[i].fitness = fitness

    for _ in range(max_generations):
        selection()
        # crossover()
        mutate()
        regenerate_tool_life()
        for i in range(total_population):
            arr = packer(population[i].individual[:], population[i].tool_life[:])
            print("Internal Arr", arr)
            fitness = calculate_fitness(arr, tsm, product_quantity)
            population[i].fitness = fitness
        print(f"Population after {_ + 1}th iteration")
        traverse_population()
        
