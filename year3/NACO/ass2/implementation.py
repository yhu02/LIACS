"""NACO assignment 22/23.

This file contains the skeleton code required to solve the first part of the 
assignment for the NACO course 2022. 

You can test your algorithm by using the function `test_algorithm`. For passing A1,
your GA should be able to pass the checks in this function for dimension=100.

## Installing requirements
You are encouraged to use a virtual environment. Install the required dependencies 
(via the command line) with the following command:
    pip install ioh>=0.3.3
"""

import random
import shutil

import ioh

##################################### START OF MY CODE #########################################################################

import numpy as np # Math library to avoid for-loops








class GeneticAlgorithm:
    """An implementation of the Genetic Algorithm."""



    ####### FACTORY FUNCTIONS #######

    ### Two versions of mutation/variation

    def flipbit_mutation_nonbinary(self,gamma=0.1):
        def f(population):
            base = self.base
            # Find out which base we are in
            # Create a bunch of ones where mutation should happen
            mut = (np.random.random(population.shape) < gamma).astype(int)
            # Define the mutation
            mut *= np.random.choice(list(range(1, base)), population.shape)
            # Perform the mutation and return it modulo the base
            return (population + mut) % base
        return f

    def flipbit_mutation(self,gamma=0.1):
        def f(population):
            mut = np.random.random(population.shape) < gamma
            return np.logical_xor(population, mut).astype(int)
        return f

    def swap_mutation(self):
        def f(population):
            x = np.hstack((np.arange(len(population)), np.arange(len(population))[::-1]))
            y = np.random.randint(0, len(population[0]), 2*len(population))
            population[x, y] = population[x, y][::-1]
            return population
        return f

    ### Two versions of environmental selection

    def truncation_environmentalselection(self,M=10):
        def f(population, fitness):
            selection = np.argsort(fitness)[-M:]
            return population[selection], np.array(fitness)[selection]
        return f

    def roulette_environmentalselection(self,M=10, repair_negs=True):
        def f(population, fitness):
            p = np.array(fitness)
            if repair_negs:
                p -= np.min(p) - 1
            selection = np.argmax(np.cumsum(p / sum(p)) > np.random.random((M, 1)), axis=1)
            return population[selection], np.array(fitness)[selection]
        return f

    ### A version of mating selection

    # It takes two other functions, one for parent selection and one for recombination

    def twoparent_matingselection(self,parent_selection, recombination, N=10, keep_old=True):
        def f(population, fitness):
            sorted_pop = population[np.argsort(fitness)][::-1]
            n_offspring = N - len(population) if keep_old else N
            selection = parent_selection(n_offspring, len(population))
            choose_first = recombination(n_offspring, len(population[0]))
            new_generation = np.where(choose_first, *sorted_pop[selection])
            return np.concatenate((population, new_generation)) if keep_old else new_generation
        return f

    # Four possible functions to give as argument to above function

    def exponential_parentselection(self,s=5):
        def f(n_pairs, n_possibilities):
            return np.random.exponential(s, (2, n_pairs)).astype(int) % n_possibilities
        return f

    def tournament_parentselection(self,t=10):
        def f(n_pairs, n_possibilities):
            return np.min(np.random.choice(range(n_possibilities), (2, n_pairs, t)), axis=2)
        return f

    def uniformcrossover_recombination(self):
        def f(n_pairs, stringsize):
            return np.random.choice((0, 1), (n_pairs, stringsize))
        return f

    def onepointcrossover_recombination(self):
        def f(n_pairs, stringsize):
            return np.arange(stringsize) >= np.random.randint(0, stringsize + 1, (n_pairs, 1))
        return f


    ##################################### END OF MY CODE #########################################################################


    def __init__(self, budget: int, k:int) -> None:
        """Construct a new GA object.

        Parameters
        ----------
        budget: int
            The maximum number objective function evaluations
            the GA is allowed to do when solving a problem.

        Notes
        -----
        *   You can add more parameters to this constructor, which are specific to
            the GA or one of the (to be implemented) operators, such as a mutation rate.
        """

        self.budget = budget
        self.base = k

    def __call__(self, problem: ioh.problem.Integer) -> ioh.IntegerSolution:
        """Run the GA on a given problem instance.

        Parameters
        ----------
        problem: ioh.problem.Integer
            An integer problem, from the ioh package. This version of the GA
            should only work on binary/discrete search spaces.

        Notes
        -----
        *   This is the main body of you GA. You should implement all the logic
            for this search algorithm in this method. This does not mean that all
            the code needs to be in this method as one big block of code, you can
            use different methods you implement yourself.

        *   For now there is a random search process implemented here, which
            is a placeholder, and just to show you how to call the problem
            class.
        """
        
        """
        # ### Random Search ###
        l = [int(i) for i in range(self.base)]
        for evaluation in range(self.budget):
        #     # Generate a random solution
            x = random.choices(l, k=problem.meta_data.n_variables)

        #     # Call the problem
            y = problem(x)
        # ### Random Search ###
        """
        
        
##################################### START OF MY CODE #########################################################################

        
        
        
        
        
        ################################# GENETIC ALGORITHM ############################
        
        ##### Define constants.
        
        M = 11 # Number of individuals in the population
        n = problem.meta_data.n_variables # dimensionality of the problem
        
        
        
        
        ##### Choose which FIVE operators to use by assigning the five variables a function each.
        #####        -- initialize_population    :                      () -> population
        #####        -- mating_selection         :   (population, fitness) -> population
        #####        -- variation                :            (population) -> population
        #####        -- environmental_selection  :   (population, fitness) -> (population, fitness)
        #####        -- termination_criteria     :         (problem.state) -> boolean
        
        initialize_population = lambda: np.random.choice((0, 1), (M, n))
        
        sel = self.exponential_parentselection(s = 1/n) # Performs GREAT
        # sel = tournament_parentselection(t = 42) # Performs GREAT
        rec = self.uniformcrossover_recombination() # Performs GREAT but doesn't matter
        # rec = onepointcrossover_recombination() # Performs GREAT but doesn't matter
        # rec = lambda x, y: 0 # EMPTY FUNCTION Performs GREAT as well
        mating_selection = self.twoparent_matingselection(sel, rec, N = 2*M, keep_old=False)
        
        MULTIPLE_OPERATORS = False
        variation = None

        if(MULTIPLE_OPERATORS == False):
            variation = self.flipbit_mutation_nonbinary(gamma = 1/n) # Performs GREAT
        # variation = swap_mutation() # Performs EXTREMELY BAD
        
        environmental_selection = self.truncation_environmentalselection(M) # Performs GREAT
        # environmental_selection = roulette_environmentalselection(M) # Performs OK
        
        termination_criteria = lambda s: s.evaluations >= self.budget or s.optimum_found or s.current_best.y >= 1.0
        
        
        
        
        
        ##### The generic algorithm
        
        # Generate a random population of solutions
        x = initialize_population()
        y = [problem(p) for p in x]

        # Compute next generations until the termination criteria are met
        while not termination_criteria(problem.state):
            # Used for experiment 3
            if(MULTIPLE_OPERATORS == True):
                if(random.randint(0,1) == 0):
                    variation = self.flipbit_mutation_nonbinary(gamma = 1/n) # Performs GREAT
                else:
                    variation = self.swap_mutation()

            x = variation(mating_selection(x, y))

            y = [problem(p) for p in x]
            x, y = environmental_selection(x, y)
            
        
        ################################# GENETIC ALGORITHM ############################
        
        
        
        
        
        
        
        
        # #### FASTER ALGORITHM THAT DOES (NOT) USE GA ####
        # y = problem(x := np.random.choice((0, 1), problem.meta_data.n_variables))
        # c, g = np.zeros((2, len(x)), dtype = int)
        # while sum(c) < len(c):
        #     g[(r := np.random.choice(np.flatnonzero((c == 0) & (g == 0))))] = 1
        #     x[r] = not x[r]
        #     c[r], x[r], g, y = (z:=problem(x))!=y, (z<y)^x[r], (z<=y)*g, max(z, y)
        # #### FASTER ALGORITHM THAT DOES (NOT) USE GA ####
        
        
        
            

        # Output the number of evaluations and return the best solution so far
        print("Number of evaluations: ", problem.state.evaluations)
        
       

##################################### END OF MY CODE #########################################################################


        
        return problem.state.current_best






def test_algorithm(dimension, instance=1):
    """A function to test if your implementation solves a OneMax problem.

    Parameters
    ----------
    dimension: int
        The dimension of the problem, i.e. the number of search space variables.

    instance: int
        The instance of the problem. Trying different instances of the problem,
        can be interesting if you want to check the robustness, of your GA.
    """

    budget = int(dimension * 5e3)
    problem = ioh.get_problem("OneMax", instance, dimension, "Integer")
    ga = GeneticAlgorithm(budget)
    solution = ga(problem)

    print("GA found solution:\n", solution)

    assert problem.state.optimum_found, "The optimum has not been reached."
    assert problem.state.evaluations <= budget, (
        "The GA has spent more than the allowed number of evaluations to "
        "reach the optimum."
    )

    print(f"OneMax was successfully solved in {dimension}D.\n")


def collect_data(dimension=100, nreps=5):
    """OneMax + LeadingOnes functions 10 instances.

    This function should be used to generate data, for A1.

    Parameters
    ----------
    dimension: int
        The dimension of the problem, i.e. the number of search space variables.

    nreps: int
        The number of repetitions for each problem instance.
    """

    budget = int(dimension * 5e2)
    suite = ioh.suite.PBO([1, 2], list(range(1, 11)), [dimension])
    logger = ioh.logger.Analyzer(algorithm_name="GeneticAlgorithm")
    suite.attach_logger(logger)

    for problem in suite:
        print("Solving: ", problem)

        for _ in range(nreps):
            ga = GeneticAlgorithm(budget)
            ga(problem)
            problem.reset()
    logger.close()

    shutil.make_archive("ioh_data", "zip", "ioh_data")
    shutil.rmtree("ioh_data")


if __name__ == "__main__":
    # Simple test for development purpose
    test_algorithm(10)

    # Test required for A1, your GA should be able to pass this!
    test_algorithm(100)

    # If your implementation passes test_algorithm(100)
    collect_data(100)