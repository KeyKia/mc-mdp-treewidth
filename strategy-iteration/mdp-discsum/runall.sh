g++ strategy-iteration.cpp -std=c++11 -osi
for filename in ../../benchmarks/dacapobenchmark/outputs/raw_outputs/*.txt; do
	name=${filename##*/};
	name=${name%.txt};
	echo "processing $name";
	mkdir mcs-$name
	python ../../mdp/mdp_utils/mdpsum_reformat.py ../../benchmarks/dacapobenchmark/outputs/raw_outputs/$name.txt mcs-$name
	totaltime=0
	for txtfile in mcs-$name/*.txt; do
		#if [[ "$txtfile" =~ [^a-zA-Z0-9_./-] ]]; then
		#	echo "Skipping invalid file" $txtfile;
		#	continue
		#fi;
		echo textfile is $txtfile
		./si $txtfile > $txtfile.log 
		v=`cat $txtfile.log`
		echo thistime is $v
		echo $v + $totaltime | bc > temp.tmp
		totaltime=`cat temp.tmp`
		echo totaltime is $totaltime
	done
	echo $name $totaltime >> results.txt
done

