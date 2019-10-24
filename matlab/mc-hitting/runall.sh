rm -r -f mcs-*
g++ convert_to_matlab_system.cpp -std=c++11 -ocnv
for filename in ../../benchmarks/dacapobenchmark/outputs/raw_outputs/*.txt; do
	name=${filename##*/};
	name=${name%.txt};
	echo "processing $name";
	mkdir mcs-$name
	python ../../mc/mc_utils/mcprob_reformat.py ../../benchmarks/dacapobenchmark/outputs/raw_outputs/$name.txt mcs-$name
	for txtfile in mcs-$name/*.txt; do
		break
		echo $txtfile
		./cnv $txtfile
		cat $txtfile.m | /Applications/MATLAB_R2019b.app/bin/matlab -nodisplay -nosplash -nodesktop >> $txtfile.log
		v=`cat $txtfile.log`
		echo $v
	done
	break
done

