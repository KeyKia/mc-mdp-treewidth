This repository contains the proof-of-concept implementations of the algorithms
described in the following paper and scripts to reproduce the results from section 4:
    A. Asadi, K. Chatterjee, A. K. Goharshady, K. Mohammadi, A. Pavlogiannis
    Faster Algorithms for Quantitative Analysis of Markov Chains and Markov
    Decision Processes with Small Treewidth


Dependencies
--------------------------------------------------------------------------------------------------
The docker container includes the packages and tools required to run the comparisons.
If you are not using the docker container, you should install the following tools
in the current directory as per your need to re-run the corresponding experiment:
    - gorubi
    - prism
    - storm
    - matlab
    - lpsolve

Compilation Instructions
--------------------------------------------------------------------------------------------------
If you are using the docker container, the implementations are already compiled and
you only need to run the .sh script in each experiment folder to reproduce its results.
Otherwise you can simply build the implemented algorithms using cmake:

```
mkdir build && cd build
cmake..
make
```

The implemented algorithms are:
    - Hitting probability computation for MCs with small treewidth under mc_hitprob/ours
    - Discounted sums of rewards computation for MCs with small treewidth under mc_discsum/ours
    - Hitting probability computation for MDPs with small treewidth under mdp_hitprob/ours
    - Discounted sums of rewards computation for MDPs with small treewidth under mdp_discsum/ours
    - Linear System of Equations solver with small treewidth of the primal graph under sle/

Running the Experiments
--------------------------------------------------------------------------------------------------
For each experiment in section 4 of the paper, you can easily find the corresponding folder by
following the folder names. In each folder, there is a .sh script which will run the experiment
if you have the required package installed.