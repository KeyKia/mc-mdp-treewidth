#!/bin/bash

for file in `ls inputs/`
do

echo start $file
obj=`echo ${file} | rev | cut -f 2- -d '.' | rev`

if [[ -e outputs/raw_outputs/${obj}.txt ]]
then
    continue
fi


/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/bin/java "-javaagent:/Applications/IntelliJ IDEA.app/Contents/lib/idea_rt.jar=51732:/Applications/IntelliJ IDEA.app/Contents/bin" -Dfile.encoding=UTF-8 -classpath /Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/charsets.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/deploy.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/cldrdata.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/dnsns.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/jaccess.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/jfxrt.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/localedata.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/nashorn.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/sunec.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/sunjce_provider.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/sunpkcs11.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/ext/zipfs.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/javaws.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/jce.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/jfr.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/jfxswt.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/jsse.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/management-agent.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/plugin.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/resources.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre/lib/rt.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/lib/ant-javafx.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/lib/dt.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/lib/javafx-mx.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/lib/jconsole.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/lib/packager.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/lib/sa-jdi.jar:/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/lib/tools.jar:/Users/ali/research/rmc-treewidth-code/benchmarks/dacapobenchmark/target/classes:/Users/ali/.m2/repository/ca/mcgill/sable/soot/3.3.0/soot-3.3.0.jar:/Users/ali/.m2/repository/commons-io/commons-io/2.6/commons-io-2.6.jar:/Users/ali/.m2/repository/org/smali/dexlib2/2.2.5/dexlib2-2.2.5.jar:/Users/ali/.m2/repository/com/google/code/findbugs/jsr305/1.3.9/jsr305-1.3.9.jar:/Users/ali/.m2/repository/com/google/guava/guava/18.0/guava-18.0.jar:/Users/ali/.m2/repository/org/ow2/asm/asm-debug-all/5.2/asm-debug-all-5.2.jar:/Users/ali/.m2/repository/xmlpull/xmlpull/1.1.3.4d_b4_min/xmlpull-1.1.3.4d_b4_min.jar:/Users/ali/.m2/repository/de/upb/cs/swt/axml/2.0.0/axml-2.0.0.jar:/Users/ali/.m2/repository/ca/mcgill/sable/polyglot/2006/polyglot-2006.jar:/Users/ali/.m2/repository/de/upb/cs/swt/heros/1.1.0/heros-1.1.0.jar:/Users/ali/.m2/repository/org/functionaljava/functionaljava/4.2/functionaljava-4.2.jar:/Users/ali/.m2/repository/ca/mcgill/sable/jasmin/3.0.1/jasmin-3.0.1.jar:/Users/ali/.m2/repository/ca/mcgill/sable/java_cup/0.9.2/java_cup-0.9.2.jar:/Users/ali/.m2/repository/org/slf4j/slf4j-api/1.7.5/slf4j-api-1.7.5.jar:/Users/ali/.m2/repository/org/slf4j/slf4j-simple/1.6.4/slf4j-simple-1.6.4.jar JTDecMain false 3 -pp -process-dir /Users/ali/research/rmc-treewidth-code/benchmarks/dacapobenchmark/inputs/$file > outputs/output.txt 2> /dev/null
if [[ $? != 0 ]]
then
    echo error $file
    continue
fi

sed '1d' outputs/output.txt | sponge outputs/output.txt
sed '$d' outputs/output.txt | sponge outputs/output.txt
sed '$d' outputs/output.txt | sponge outputs/output.txt

cp outputs/output.txt outputs/raw_outputs/${obj}.txt
mkdir outputs/${obj}

python outputs/transform.py outputs/output.txt outputs/${obj}

done
