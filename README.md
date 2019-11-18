# dc_splp

dc_splp is a Simple plant location problem (SPLP; a.k.a Uncapacitated Facility Location (UFL)) and p-median solver that uses a search method named Disperse Construction.

A newer, more advanced version can be found at https://github.com/autopawn/dc_splp .

Disperse Construction is a **constructive** method for solving combinatorial problems.
On each step, solutions are grown adding one element on each possible way, then `pool_size` solutions are selected from these (`pool_size` is a parameter given by the computational resources avaialable).

Unlike beam-search this selection doesn't just consider the solution values, but also the dissimilitude between them given a dissimilitude metric.
Solutions too similar are discarded in the **reduction** process. 

**Local search** is applied on the final phase of the algorithm for all the solutions using an efficient exchange heuristic.
Exploring diverse solutions (that also are good) allows to reach different local optima, increasing the chances of finding a global optima.

# Variants

The [`Makefile`](./Makefile) compiles several variants of the program, each one performs a different **reduction** process.
The most important are:

| Version  | Reduction process |
| :------- | ----------- |
| `dc_bes` | Just selects the best solutions (beam-search). |
| `dc_ran` | Just selects solutions at random. |
| `dc_dismsemin` | M.G.E. for dissimilitude, **minimum_triangle** as facility-facility distance |
| `dc_dismsesum` | M.G.E. for dissimilitude, **sum_of_deltas** as facility-facility distance |
| `dc_dishaumin` | Hausdorf for dissimilitude, **minimum_triangle** as facility-facility distance |
| `dc_dishausum` | Hausdorf for dissimilitude, **sum_of_deltas** as facility-facility distance |
| `dc_discli`    | **per_client_delta** as dissimilitude |

Also, for each one of these, a version that appends an `L` to the name is created. This `L` version is intentended to handle larger problems.

* `dc_dismsemin` is recommended for problems on a metric space.
* `dc_dismsesum` is recommended for problems outside a metric space.
* `dc_ran` is a fast option.

## Macros and limits

The program is compiled and optimized to solve problems of a fixed sizes, this size is given by macros that can be changed on the begining of the [`Makefile`](./Makefile):

| Variable  | Description |
| :------- | ----------- |
| `SMALL_SOL_SIZE` | Limit to the solution sizes (depth of the tree). |
| `SMALL_N` | Limit for the number of facilities. |
| `SMALL_M` | Limit for the number of clients. |
| `LARGE_SOL_SIZE` | Limit to the solution sizes for the `L` version. |
| `LARGE_N` | Limit for the number of facilities for the `L` version. |
| `LARGE_M` | Limit for the number of clients for the `L` version. |
| `THREADS` | Number of threads that the program will use. |

If the limits specified by the macro are smaller than the size of the problem, the program will throw a runtime error.

## Compilation

In order to compile, execute:
```bash
make
```
the program variants will be compiled to the `bin/` folder.

## Execution

The program variants are called as follows:

```bash
./bin/<variant> <pool_size> <vision_range> <max_sols_to_show> <problem_file> <output_before_ls> <output_after_ls>
```
Where:

| Argument  | Description | Example |
| :------- | ----------- | :--------: |
| `<variant>` | The variant of the program to be run. | `dc_dismsemin` |
| `<pool_size>` | How many solutions to be selected on each level. | `200` |
| `<vision_range>` | Parameter for the quality of the reduction. | `400` |
| `<max_sols_to_show>` | Number best solutions to be shown and saved. | `10` |
| `<problem_file>` | File that has the problem description | `pmedian/pmed26.txt` |
| `<output_before_ls>` | File to save solutionsa before local search | `output.txt` |
| `<output_after_ls>` | File to save solutionsa after local search | `output_ls.txt` |

Some variants like `dc_ran` and `dc_bes` don't use the `<vision_range>` parameter, so a `0` should be specified.

Larger values of `<pool_size>` often require considerable memory and time. A good rule-of-thumb for the value of `<vision_range>`, when needed, is `2*<pool_size>`.

## Supported formats

The program supports two formats, both specified for the [UflLib benchmark](https://resources.mpi-inf.mpg.de/departments/d1/projects/benchmarks/UflLib/data-format.html):

* ORLIB-cap format: only supports uncapacitated facility location.
* UflLib Simple format: supports both p-median and uncapacitated facility location.
  The [`pmedian`](./pmedian) folder contains translations to this format of the [ORLIB p-median](http://people.brunel.ac.uk/~mastjjb/jeb/orlib/pmedinfo.html) problems.

The UflLib problems can be found in the [dc_splp_results](https://github.com/autopawn/dc_splp_results) repository:
* [pmedian](https://github.com/autopawn/dc_splp_results/tree/master/pmedian)
* [pmedianlarge](https://github.com/autopawn/dc_splp_results/tree/master/pmedianlarge)
* [splp](https://github.com/autopawn/dc_splp_results/tree/master/splp)
* [splpkmedian](https://github.com/autopawn/dc_splp_results/tree/master/splpkmedian)
