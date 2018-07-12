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
- The criteria for updating the reference set, when necessary, must be based on comparisons and measures of diversity between the new solutions and the existing solutions.
<!-- NOTE: Is effective to combine local optima? -->
<!-- NOTE: Combination operator and stop criteria are to be determined. -->
