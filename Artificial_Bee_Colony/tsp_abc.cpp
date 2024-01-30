#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

using namespace std;

// Function to calculate the total distance of a tour
double calculate_distance(const vector<int> &tour, const vector<vector<double>> &distance_matrix)
{
    double distance = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i)
    {
        distance += distance_matrix[tour[i]][tour[i + 1]];
    }
    distance += distance_matrix[tour.back()][tour[0]]; // Return to the starting city
    return distance;
}

// Function to initialize a random solution
vector<int> generate_solution(size_t num_cities)
{
    vector<int> solution(num_cities);
    iota(solution.begin(), solution.end(), 0);            // Fill with 0, 1, 2, ..., num_cities-1
    random_shuffle(solution.begin() + 1, solution.end()); // Shuffle, excluding the first element
    return solution;
}

// Function to evaluate the fitness of a solution
double evaluate_solution(const vector<int> &solution, const vector<vector<double>> &distance_matrix)
{
    return 1 / calculate_distance(solution, distance_matrix);
}

// Artificial Bee Colony algorithm for TSP
pair<vector<int>, double> abc_tsp(const vector<vector<double>> &distance_matrix, size_t num_bees, size_t num_iterations)
{
    size_t num_cities = distance_matrix.size();

    // Initialize solutions with random tours
    vector<vector<int>> solutions(num_bees);
    for (auto &solution : solutions)
    {
        solution = generate_solution(num_cities);
    }

    // Evaluate the fitness of each solution
    vector<double> fitness(num_bees);
    transform(solutions.begin(), solutions.end(), fitness.begin(),
              [&](const vector<int> &solution)
              { return evaluate_solution(solution, distance_matrix); });

    for (size_t iteration = 0; iteration < num_iterations; ++iteration)
    {
        // Employed bees phase
        for (size_t i = 0; i < num_bees; ++i)
        {
            // Generate a random solution index different from i
            size_t j;
            do
            {
                j = rand() % num_bees;
            } while (j == i);

            // Generate a new solution by combining the current solution and the randomly selected solution
            vector<int> new_solution = solutions[i];
            size_t position = rand() % num_cities;
            copy(solutions[j].begin() + position, solutions[j].end(), new_solution.begin() + position);

            // Evaluate the fitness of the new solution
            double new_fitness = evaluate_solution(new_solution, distance_matrix);

            // Update the solution if the new solution is better
            if (new_fitness > fitness[i])
            {
                solutions[i] = new_solution;
                fitness[i] = new_fitness;
            }
        }

        // Onlooker bees phase
        vector<size_t> onlooker_indices(num_bees);
        iota(onlooker_indices.begin(), onlooker_indices.end(), 0);
        shuffle(onlooker_indices.begin(), onlooker_indices.end(), default_random_engine());

        for (size_t i : onlooker_indices)
        {
            size_t j;
            do
            {
                j = rand() % num_bees;
            } while (j == i);

            vector<int> new_solution = solutions[i];
            size_t position = rand() % num_cities;
            copy(solutions[j].begin() + position, solutions[j].end(), new_solution.begin() + position);

            double new_fitness = evaluate_solution(new_solution, distance_matrix);

            if (new_fitness > fitness[i])
            {
                solutions[i] = new_solution;
                fitness[i] = new_fitness;
            }
        }

        // Scout bees phase
        auto max_fitness_it = max_element(fitness.begin(), fitness.end());
        size_t max_fitness_index = distance(fitness.begin(), max_fitness_it);

        if (*max_fitness_it == 0)
        {
            solutions[max_fitness_index] = generate_solution(num_cities);
            fitness[max_fitness_index] = evaluate_solution(solutions[max_fitness_index], distance_matrix);
        }
    }

    // Return the best solution found
    auto max_fitness_it = max_element(fitness.begin(), fitness.end());
    size_t best_solution_index = distance(fitness.begin(), max_fitness_it);
    return make_pair(solutions[best_solution_index], calculate_distance(solutions[best_solution_index], distance_matrix));
}

int main()
{

    vector<vector<double>> distance_matrix = {{0, 2, 9, 10}, {1, 0, 6, 4}, {15, 7, 0, 8}, {6, 3, 12, 0}};

    size_t num_cities = distance_matrix.size();
    size_t num_bees = 20;
    size_t num_iterations = 100;

    auto result = abc_tsp(distance_matrix, num_bees, num_iterations);

    cout << "Best Tour: ";
    for (int city : result.first)
    {
        cout << city << " ";
    }
    cout << endl;

    cout << "Best Distance: " << result.second << endl;

    return 0;
}