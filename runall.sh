
red=`tput setaf 1`
green=`tput setaf 2`
yellow=`tput setaf 3`
blue=`tput setaf 4`
reset=`tput sgr0`

echo "${blue}---------------------------- Computing Hitting Probabilities in Markov Chains ----------------------------${reset}"
cd mc_hitprob/ours/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../gauss/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../prism/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../storm/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../../

echo "${blue}---------------------------- Computing Expected Discounted Sums in Markov Chains ----------------------------${reset}"
cd mc_discsum/ours/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../gauss/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../../

echo "${blue}---------------------------- Computing Hitting Probabilities in Markov Decision Processes ----------------------------${reset}"
cd mdp_hitprob/ours/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../lpsolve/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../prism/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../storm/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../strategy-iteration/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../value-iteration/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../../

echo "${blue}---------------------------- Computing Expected Discounted Sums in Markov Decision Processes ----------------------------${reset}"
cd mdp_discsum/ours/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../lpsolve/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../strategy-iteration/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../value-iteration/
rm -rf mdps* >/dev/null
rm -rf mcs* >/dev/null
rm -f results.txt
./run.sh
cd ../../

python3 plotResults.py