# Assignment 1
# [1] Implement the simple genetic algorithm in a programming language of your choice.
# [2] Use the implemented genetic algorithm to find an individual with all 1s - OneMax problem.
# [3] Change the fitness of the algorithm to find an individual with alternating 1s and 0s.
# [4] Try to change the parameters (probability of the mutation/crossover) and see what happens.
# [5] Submit a plot comparing the convergence of the algorithm for two different settings of the algorithm.
# [6] Explain what you did.


# [1]
import matplotlib.pyplot as plt
import random

POP_SIZE = 100
CHROMO_LEN = 25
ITER_NUM = 1000
MUT_CHANCE = 0.01
CROSS_PROB = 0.7

def create_individual():
    return [random.randint(0, 1) for _ in range(CHROMO_LEN)]

def create_random_population():
    rand_pop = []
    for i in range(POP_SIZE):
        rand_pop.append(create_individual())
    return rand_pop

def calculate_fitness_individual(individual):
    nr1 = 0
    for i in individual:
        if i == 1:
            nr1 += 1
    return nr1

def calculate_fitness_individual_alternating(individual):
    fit = 0
    for i in range(len(individual) - 1):
        if (individual[i] == 0 and individual[i+1] == 1) or (individual[i] == 1 and individual[i+1] == 0):
            fit += 1
    return fit

def calculate_fitness(pop):
    fit = []
    for i in range(POP_SIZE):
        fit.append(calculate_fitness_individual(pop[i]))
    return fit

def selection(pop, fit):
    selected = random.choices(pop, weights=fit, k=POP_SIZE)
    mate_pool = [(selected[i], selected[i+1]) for i in range(0, POP_SIZE, 2)]
    return mate_pool

def crossover(parent1, parent2):
    if random.random() > CROSS_PROB:
        return parent1, parent2
    crossover_point = random.randint(0, CHROMO_LEN-1)
    offspring1 = parent1[:crossover_point] + parent2[crossover_point:]
    offspring2 = parent2[:crossover_point] + parent1[crossover_point:]
    return offspring1, offspring2

def mutation(individual):
    for i in range(CHROMO_LEN):
        if random.random() < MUT_CHANCE:
            individual[i] = 1 - individual[i]
    return individual

def operators(mate_pool):
    off_spring = []
    for i in range(POP_SIZE//2):
        offspring1, offspring2 = crossover(mate_pool[i][0], mate_pool[i][1])
        offspring1 = mutation(offspring1)
        offspring2 = mutation(offspring2)
        off_spring.append(offspring1)
        off_spring.append(offspring2)
    return off_spring

def run_genetic_algorithm(mut_chance, cross_prob):
    global MUT_CHANCE, CROSS_PROB
    MUT_CHANCE = mut_chance
    CROSS_PROB = cross_prob

    population = create_random_population()
    fitness_history = []

    for _ in range(ITER_NUM):
        fitness = calculate_fitness(population)
        fitness_history.append(sum(fitness) / POP_SIZE)
        mating_pool = selection(population, fitness)
        population = operators(mating_pool)

    return fitness_history

if __name__ == '__main__':
    fitness_history1 = run_genetic_algorithm(0.01, 0.7)
    fitness_history2 = run_genetic_algorithm(0.1, 0.3)

    plt.plot(fitness_history1, label='Mutate% = 0.01 | Cross% = 0.7')
    plt.plot(fitness_history2, label='Mutate% = 0.1 | Cross% = 0.3')
    plt.xlabel('Iteration')
    plt.ylabel('Average fitness')
    plt.legend()
    plt.show()

'''
Explanation:
I have implemented the simple genetic algorithm ([1]). It works by choosing a random population initially, then going 
for however many iterations we wish, what we do is calculate each individual's fitness according to a fitness function 
that we chose, followed then by creating a mating pool. This is done by pairing up individuals randomly, making 
individuals which have a higher fitness be more likely to be paired up. Afterwards, we perform a crossover and a 
mutation on the better fit half of pairs of parents, if chances are in our favor. We then replace the initial population
with the new, hopefully better population. I have made it such that it solves the OneMax problem ([2]) and the 
alternating 1s and 0s problem ([3]), by choosing the fitness function accordingly. I have also tried changing the 
mutation and crossover probabilities ([4]). When making the Mutate% higher or Cross% lower, I noticed that the average 
fitness does not really increase, instead stagnating around 50% of the best outcome. I then plotted the convergence of 
the algorithm for two different settings of the algorithm ([5]).
'''