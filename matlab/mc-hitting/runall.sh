g++ convert_to_matlab_system.cpp -std=c++11 -ocnv
for filename in ../../benchmarks/dacapobenchmark/outputs/raw_outputs/*.txt; do
	name=${filename##*/};
	name=${name%.txt};
	echo "processing $name";
	mkdir mcs-$name
	python ../../mc/mc_utils/mcprob_reformat.py ../../benchmarks/dacapobenchmark/outputs/raw_outputs/$name.txt mcs-$name
	totaltime=0
	for txtfile in mcs-$name/*.txt; do
		echo $txtfile
		./cnv $txtfile
		cat $txtfile.m | /Applications/MATLAB_R2019b.app/bin/matlab -nodisplay -nosplash -nodesktop >>/dev/null 2> $txtfile.log 
		v=`cat $txtfile.log`
		echo thistime is $v
		echo $v + $totaltime | bc > temp.tmp
		totaltime=`cat temp.tmp`
		echo totaltime is $totaltime
	done
	echo $name $totaltime >> results.txt
done

