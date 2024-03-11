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
        print("min tool life: ", min_tl)
        set_of_indexes = set(indexes)
        for i in set_of_indexes:
            arr[i][2]-=min_tl

        print("Array after reducing tool lifes: ", arr)
        print("Selected indexes", indexes)
        print("Set of indexes", set_of_indexes)
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

        print("Cost is: ", cost)
        fitness += ((cost)*min_tl)
        print("Fitness at this step: ", fitness)
        jobs-=min_tl
        
    return fitness



tsm = [0, 1, 5, 4, 6, 4, 5, 6, 3, 2]
ind = [3, 6, 4, 5, 1,
       0, 1, 3, 0, 1,
       0, 1, 5, 4, 6, 
       3, 3, 2, 0, 1,
       0, 1, 0, 1, 5,
       4, 6, 3, 2, 3,
       3, 2, 3, 3, 6,
       4, 5, 1, 0, 1,
       0, 1, 0, 3, 2]
tl = [326, 326, 326, 326, 131,
      154, 131, 326, 154, 131,
      154, 131, 326, 326, 326,
      326, 326, 326, 154, 131,
      154, 131, 154, 131, 326,
      326, 326, 326, 326, 326,
      326, 326, 326, 326, 326,
      326, 326, 131, 154, 131,
      154, 131, 154, 326, 326]
jobs = 1304

arr = []
for i, a in enumerate(zip(ind, tl)):
    arr.append([i, a[0], a[1]])

print("Array Start", arr)

print("\n\n")
fitness = calculate_fitness(arr, tsm, jobs)
print(fitness)