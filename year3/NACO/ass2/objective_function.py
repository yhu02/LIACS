import typing
import shutil
import random
import numpy as np
import ioh
from implementation import GeneticAlgorithm
import csv
import ast

class CellularAutomata:
    r:int = 1
    # Hardcoded k = 2 or k = 3 for the experiments################################################################################################################
    # To not mess with template code
    def __init__(self, rule_number: int, k:int = 2):
        """Intialize the cellular automaton with a given rule number"""
        # Fill dict  with numbers with base k
        def getRuleSetDict(rule_number: int):
            length = k ** (2 * self.r + 1)
            rule_set = [int(i) for i in np.base_repr(rule_number, k).zfill(length)]

            rule_dict = {}
            for i, result in enumerate(reversed(rule_set)):
                pattern = np.base_repr(i, k).zfill(2 * self.r + 1)
                rule_dict[pattern] = result

            return rule_dict
        self.rule_dict = getRuleSetDict(rule_number)

    #Calculate the next state for all cells
    def calculate(self, c0):
        copy = np.pad(c0, [self.r, self.r])
        for i in range(len(c0)):
            result = copy[i : i + (2 * self.r + 1)]
            result = "".join([str(j) for j in result])
                
            c0[i] = self.rule_dict[result]
        return c0



    def __call__(self, c0: typing.List[int], t: int) -> typing.List[int]:
        """Evaluate for T timesteps. Return Ct for a given C0."""
        for i in range(t):
            c0 = self.calculate(c0)
        return c0

def make_objective_function(ct, rule, t, similarity_method):
    '''Create a CA objective function.'''
    
    if similarity_method == 1:
        def similarity(ct: typing.List[int], ct_prime: typing.List[int]) -> float:
            # Hamming
            counter = 0
            for i in range(len(ct)):
                if(ct[i] == ct_prime[i]):
                    counter += 1
            return counter/len(ct)
    else:
        def similarity(ct: typing.List[int], ct_prime: typing.List[int]) -> float:
            # Levenshtein
            # https://blog.paperspace.com/implementing-levenshtein-distance-word-autocomplete-autocorrect/
            def levenshteinDistanceDP(token1, token2):
                distances = np.zeros((len(token1) + 1, len(token2) + 1))

                for t1 in range(len(token1) + 1):
                    distances[t1][0] = t1

                for t2 in range(len(token2) + 1):
                    distances[0][t2] = t2
 
                a = 0
                b = 0
                c = 0

                for t1 in range(1, len(token1) + 1):
                    for t2 in range(1, len(token2) + 1):
                        if (token1[t1-1] == token2[t2-1]):
                            distances[t1][t2] = distances[t1 - 1][t2 - 1]
                        else:
                            a = distances[t1][t2 - 1]
                            b = distances[t1 - 1][t2]
                            c = distances[t1 - 1][t2 - 1]
                            
                            if (a <= b and a <= c):
                                distances[t1][t2] = a + 1
                            elif (b <= a and b <= c):
                                distances[t1][t2] = b + 1
                            else:
                                distances[t1][t2] = c + 1

                return distances[len(token1)][len(token2)]
            distance = levenshteinDistanceDP(ct, ct_prime)

            return 1-(distance/len(ct))

    
    def objective_function(c0_prime: typing.List[int]) -> float:
        """Skeleton objective function. 
        
        You should implement a method  which computes a similarity measure 
        between c0_prime a suggested by your GA, with the true c0 state 
        for the ct state given in the sup. material.

        Parameters
        ----------
        c0_prime: list[int] | np.ndarray
            A suggested c0 state
        
        Returns
        -------
        float
            The similarity of ct_prime to the true ct state of the CA           
        """
        ca = CellularAutomata(rule)
        ct_prime = ca(c0_prime, t)
        return similarity(ct, ct_prime)

    return objective_function

def example(nreps=5):
    with open('ca_input.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            if line_count != 0:
                k = int(row[0]) #NOTE in the class CellularAutomata, you have to manually change the k
                rule = int(row[1])
                t = int(row[2])
                ct = np.asarray(ast.literal_eval(row[3]))
                """An example of wrapping a objective function in ioh and collecting data
                for inputting in the analyzer."""

                # Create an objective function
                objective_function = make_objective_function(ct, rule, t, 2)
                
                # Wrap objective_function as an ioh problem
                problem = ioh.wrap_problem(
                    objective_function,
                    name="objective_function_ca_1", # Give an informative name 
                    dimension=len(ct), # Should be the size of ct
                    problem_type="Integer",
                    optimization_type=ioh.OptimizationType.MAX,
                    lb=0,
                    ub=k - 1,         # 1 for 2d, 2 for 3d
                )
                # Attach a logger to the problem
                logger = ioh.logger.Analyzer()
                problem.attach_logger(logger)

                # run your algoritm on the problem
                for _ in range(nreps):
                    algorithm = GeneticAlgorithm(10000, k)
                    algorithm(problem)
                    problem.reset()
                
                logger.close()

                shutil.make_archive(f"ioh_data{line_count}", "zip", f"ioh_data{line_count}")
            
            line_count += 1


if __name__ == "__main__":
    example()
