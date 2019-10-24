g++ convert_to_matlab_system.cpp -std=c++11 -ocnv
for filename in ../../benchmarks/dacapobenchmark/outputs/raw_outputs/*.txt; do
	name=${filename##*/};
	name=${name%.txt};
	echo "processing $name";
	mkdir mcs-$name
	python ../../mc/mc_utils/mcdiscsum_reformat.py ../../benchmarks/dacapobenchmark/outputs/raw_outputs/$name.txt mcs-$name
	totaltime=0
	for txtfile in mcs-$name/*.txt; do
		if [[ "$txtfile" =~ [^a-zA-Z0-9_./-] ]]; then
			echo "Skipping invalid file" $txtfile;
			continue
		fi;
		echo textfile is $txtfile
		./cnv $txtfile	
		txtfilename=${txtfile%.txt};
		matlabfilename=$txtfilename.m
		echo "matlab file name is" $matlabfilename
		#/Applications/MATLAB_R2019b.app/bin/matlab -nodisplay -nosplash -nodesktop -r $txtfile.m 2> $txtfile.log 
		/Applications/MATLAB_R2019b.app/bin/matlab -nodisplay -nosplash -nodesktop -r "run('$matlabfilename');exit;" >/dev/null 2> $txtfile.log 
		v=`cat $txtfile.log`
		echo thistime is $v
		echo $v + $totaltime | bc > temp.tmp
		totaltime=`cat temp.tmp`
		echo totaltime is $totaltime
	done
	echo $name $totaltime >> results.txt
done

