# Quantitative Analysis of Markov Chains and Markov Decision Processes with Small Treewidth

This repository contains proof-of-concept implementations of the algorithms
described in the paper: 
[Faster Algorithms for Quantitative Analysis of Markov Chains and Markov Decision Processes 
with Small Treewidth](https://hal.archives-ouvertes.fr/hal-02534237)
and necessary scripts to replicate the experiments.

The implemented methods contained in this repository are:
- Discrete-time Markov chains
  - Hitting probability computation for DTMCs with small treewidth
  - Discounted sums of rewards computation for DTMCs with small treewidth
- Markov Decision Processes
  - Hitting probability computation for MDPs with small treewidth
  - Discounted sums of rewards computation for MDPs with small treewidth
- Linear System of Equations solver given small treewidth of the primal graph


In addition, this also contains conversion scripts to operate other solvers and model-checkers, 
included as submodules.  If you make use of them, please cite the corresponding paper.

Structure of the repository
---------------------------
- `mc_hitprob` and `mc_discsum` contain the implementations of the proposed algorithms for DTMCs 
under `ours/`
- `mdp_hitprob` and `mdp_discsum` contain the implementations of the proposed algorithms for MDPs 
under `ours/`
- The so far mentioned directories also contain scripts to run other solvers or model-checkers 
for the corresponding task on the dacapo benchmark suite
- `sle` contains the low-treewidth system of linear equations solver
- `commons` contains the tree decomposition and other implementations common to all approaches
- `utils` contains scripts for converting inputs formats
- `benchmarks` contains the dacapo benchmark suite used for the comparisons

Using the Docker container
---------------------------
The easiest way to replicate the results of the paper is to use the prepared docker container
where the code is pre-compiled and the required tools are installed.
Run the following in order to obtain the container :

```bash
$ docker pull keykia/mc-mdp-treewidth
$ docker run -it keykia/mc-mdp-treewidth
```

After starting the container, you can find the code under `/home/mc-mdp-treewidth/artifact`.
Simply run `./runall.sh` to replicate all the results of the paper. The results will be saved
as a plot called `results.png` in the root directory. 

Please note that the experiments take quite some time to run, you may remove some of the 
benchmarks under `benchmars/dacapobenchmark/outputs/raw_outputs` if you only want a subset 
of benchmarks. Also, experiments requiring gorubi or matlab are excluded from the container 
as they are licensed software.

Compiling the code and running the experiments
---------------------------
If you are not using the docker container, you need to compile the code and install the
dependencies required for running the comparisons. You may install the following tools
as per the experiments you need:
- [prism model-checker](https://www.prismmodelchecker.org/download.php)
- [storm model-checker](http://www.stormchecker.org/getting-started.html)
- [lpsolve](https://launchpad.net/ubuntu/+source/lp-solve)
- [Gurobi solver](http://www.gurobi.com/) (Gurobi provides academic licenses 
[here](http://www.gurobi.com/academia/for-universities))
- [MATLAB](https://www.mathworks.com/products/matlab.html)

The implemented approaches can be compiled using cmake in a straight-forward manner:

```bash
$ mkdir build && cd build
$ cmake..
$ make
```

Now, simply run the `run.sh` script in each experiment directory to run it on the dacapo benchmark
or run the `runall.sh` script in the root directory to run all the experiments and plot the results.