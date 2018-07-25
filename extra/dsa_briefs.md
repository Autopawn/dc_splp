# p-median:

-

# SPLP:

- Is NP-Complete: jakob1983simple
-

@article{jakob1983simple,
  title={The simple plant location problem: Survey and synthesis},
  author={Jakob, KRA and Pruzan, Peter Mark},
  journal={European journal of operational research},
  volume={12},
  pages={36--81},
  year={1983}
}



# mlandenovic2007p

The p-median problem: A survey of metaheuristic approaches

- Is one of the basic models in discrete location theory (and NP-hard).
- Metaheuristics are frameworks for building heuristics. Heuristics are useful for get approximate solutions when the optimal cannot be computed in acceptable time.
- **Interesting related journals**.
- Significant advances on the state-of-the-art may be attributed to new metaheuristics.
- p-median may be a powerful tool for data mining applications.
- **OR-Library instances**, test p-median problems solved exactly: http://people.brunel.ac.uk/~mastjjb/jeb/info.html
    - And several more test instances!
- **Lista de heurísticas y metaheurísticas**.
- Alternate heuristic: solve 1-median on each set of clients reached by each facility and move them.
- Interchange heuristic: facilities moved iteratively, one by one.
    <!-- NOTE: Several variants? -->
    - Commonly used for comparison.
    - Common used alone or within metaheuristics.
    - Withaker did the fast interchange heuristic (1983), but wasn't used until Hansen and Mladenovic applied as a subroutine of variable neighborhood search (VNS) heuristic. Resende and Werneck offered a significantly faster in practice implementation (2003).
        - Sparse matrix proven to be more efficient in euclidean instances.
        <!-- NOTE: Possible correlation with integer-friendliness? -->
    - LK neighborhood structure (2005) was a step forward.
- Garcı́a-López et al. (2003) design a Scatter Search for the PMP by introducing a distance in the solution space.
- The use of metaheuristics has led to substantial improvements in solution quality on large
scale instances within reasonably short computing time.

<!-- NOTE: Add parallelization to future work. -->

# Parallelization of the scatter search for the p-median problem

- Scatter search (SS) metaheuristic (Glover et al., 2000) is an evolutionary strategy based on keeping reference set of good and diverse solutions that are combined to construct others exploiting the knowledge of the problem at hand, this solutions are the ones with best objective values as well as disperse solutions. They are considered disperse if they reach different local minima after performing local search.
- SS is a population-based method that constructs solutions by combining others.
- The SS reference set is updated depending on the results of the LS improvements.
- We test them with large instances of the p-median problem obtained from the **TSPLIB**.
- Three types of parallelization have been proposed to achieve either an increase of efficiency or an increase of exploration.
- The criteria for updating the reference set, when necessary, must be based on comparisons and measures of diversity between the new solutions and the existing solutions.
<!-- NOTE: Is effective to combine local optima? -->
<!-- NOTE: Combination operator and stop criteria are to be determined. -->

- The initial population must be a wide set of disperse solutions. However, it must also include good solutions. Several strategies can be applied to get a population with these properties.
- The initial population can also be obtained by a procedure that provides at the same time disperse and good solutions like **GRASP procedures**.
- Disperse solutions should reach different local minima by the local search.
- The distance matrix was taken from the instance TSPLIB RL1400 that includes 1400 points. The sets of instances are characterized with the number n of points (1400) and the number p of facility points or medians that is reported in first column of Tables 1–3 going from 10 to 100.
- The criteria for updating the reference set, when necessary, must be based on comparisons and measures of diversity between the new solutions and the existing solutions.
- The metaheuristic strategy includes the decision on how to update the reference
set taking into account the state of the search. The algorithm must also realize when
the reference set does not change and seek to diversify the search by generating a new
set of solutions for the population. In addition, the metaheuristic includes the stop-
ping criterion for the whole search procedure. Then the best solution used in the refe-
rence set is provided by the method.
- The p-median problem is NP-hard [9,21]. Many heuristics and exact methods have been proposed to solve it.
- The most time consuming part is the local search.

-The objective values found with these algorithms are comparable with the best ob-
tained in the literature [8,18]

The p-median problem is NP-hard [9,21]. Many heuristics and exact methods have been proposed to solve it. Exact algorithms are provided by Beasley [1], Cornuejols et al. [3], Galva o [7], Hanjoul and Peeters [19] and Rosing et al. [34], among others.

Classical heuristics for this problem often cited in the literature are Greedy by Kuehn and Hamburger [22], Alternate by Maranzana [26] and Interchange by Teitz and Bart [39]. Heuristics based on linear programming have been proposed by du Merle at al. [4] and by Senne and Lorena [37,38]. Another type of heuristics suggested in the literature is based on the relaxed dual of the integer programming formulation of the p-median problem found in the papers by Erlenkotter [5], Galva o [7] and Nauss and Markland [28]. A 1-interchange move is extended into a so-called 1-chain-substitution move, which is applied to p-median problem by Maldenovic et al.[27]. Tabu Search heuristics are suggested by Voss [40], where some variants of the so-called reverse elimination method are discussed and by Rolland et al. [36]. Other heuristic method appield to the p-median problem are the Heuristic Concentration by Rosing and Revelle [35] and the GRASP with path-relinking Resende and Werneck [31].

The Variable Neighborhood Search heuristic and its variants have also been applied to the p-median problems by Hansen and Mladenovic [15] and Hansen, et al. [18]. In addition, several hybrids of these heuristics have been suggested. Several parallelization methods of the Variable Neighborhood Search are considered for the p-median problem by García et al. [8]. Other heuristic procedures have been proposed by Hodgson [20], Resende and Werneck [32], Rosing [33] and Whitaker [41].

<!-- NOTE: The creation of diverse solutions is a mess.  -->
<!-- NOTE: Updating of reference sets is a mess. -->
<!-- NOTE: The reduction process could be used as part of other algorithms its usage is justified as eventually separated solutions will reach less than |VR| distance (if |VR|>PZ) -->

# Complement to a comparative analysis of heuristics for the p-median problem

- The purpose of the present note is to complete the comparative study of Alba and Domínguez (2006) by gathering or providing:
    1. Exact optimal values to test problems, in order to evaluate precisely the errors made by the heuristics
    2. Results obtained with several Variable Neighborhood Search (VNS) heuristics for the two main sets of test problems of that paper.

- A well-known data set for PMP, with 40 test problems, has been provided by Beasley (1985). They have n=100 to 900 points of which p = 5 to 200 are to be chosen. Exact solutions for all of them were obtained with a **branch-and-bound** algorithm due to that author on a Cray-1S parallel computer.

- The instances with n = 1400 of the TSP-Lib (Reinelt 1991) have been used as a data set for the PMP by several researchers. Exact solutions for instances with p = 10,20,...,70 and 90 were obtained previously with an algorithm based on stabilized column generation.

- Comparing performance of evolutionary and neural network algorithms on one side and of VNS, HYB, and scatter search on the other, on the two main sets of test problems very clearly shows the superiority of the latter group over the former one in terms of accuracy of the solutions obtained.


# Deterministic and Randomized Heuristic Algorithms for Uncapacitated Facility Location Problem

- Given a set of customers and a set of potential facilities, the objective of UFLP is to open a sub-set of the potential facilities such that sum of the opening cost for opened facilities and the service cost of customers is minimized.

- UFLP is also known as the Simple Plant Location Problem (SPLP) [1, 6] and the Warehouse Location Problem (WLP) [7].
- UFLP is known to be an NP-hard problem [8, 9].
- Some of the approaches are branch-and-bound algorithm [10, 11], tabu search [4, 5], constant factor approximation algorithm [12], greedy heuristic [13], neighborhood search [14], hybrid multi-start heuristic [15], semi-Lagrangian relaxation [16], message-passing [17], surrogate semi-Lagrangian dual [18], discrete unconscious search [19], etc.
- BFR is the acronym for backward–forward–replacement phase.

- The randomized algorithm achieves optimal results for all the instances of UFLP considered in this paper including those for which the deterministic algorithm fails to achieve the optimal solutions.

- For most of the instances, the deterministic BFR gives optimal or near-optimal results. The randomized BFR has been found to provide optimal results for all the instances where the deterministic BFR fails to give optimal results. It is to be noted that the result found by the randomized BFR is always better
or at least same as the result obtained by the deterministic BFR. For future work, the
effects of the three phases used in the proposed algorithms on the final result can be
analyzed and more experiments on other UFLP instances available in the literature
can be performed.




# Stuff

<!-- NOTE: What about considering the sorting to be RANDOM!! -->
<!-- NOTE: Fast clustering heuristics -->

<!-- NOTE: A good way to ensure diversity is ensuring diversity in the construction process -->

<!-- NOTE: Meta-evolution characteristics are a bad thing (are they a bad thing or just they aren't being used?) if solutions are only evaluated by their phenotype, however may be useful
 -->

<!-- NOTE: no need into thinking the solution representation, a mere sorted array is needed -->

<!-- NOTE: need of the distance metric! -->

<!-- NOTE: specially useful for submodular problems -->

<!-- NOTE: The LC is not done on each step as it doesn't make sense to do so while constructing the solution  -->

<!-- TODO: as will be discussed further, more than one solution is returned because they are different and may be useful for decision making.
-->

<!-- TODO: the algorithm is better for solutions of small size as the dissimilitude calculation cost diminishes.
-->


Dissimilitude construction
