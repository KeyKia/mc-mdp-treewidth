#!/bin/bash

benchs=$(ls -1 input/)

for bench in $benchs
do
  files=$(ls -1 input/$bench)
  for file in $files
  do
    if [[ $file == *"$bench"* && $file == *.txt ]]; then
      echo $file
      outfile="${file/$bench/graph}"
      if [[ -f input/$bench/$outfile ]]; then
        continue
      fi

      python transform.py input/$bench/$file
      timeout 180 ./flow_cutter_pace17 < inp.txt > ans.txt
      python treewidth.py ans.txt
      python join.py

      cp out.txt input/$bench/$outfile
    fi
  done
done