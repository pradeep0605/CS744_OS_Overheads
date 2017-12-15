#!/bin/bash
#  [--add | --list-apps | --clear app_name | --clear-all | --ctrl #number | --add-stats | --clear-stats |
# --clear-all-stats | --reset-stats | --reset-all-stats | --list-stats]
#--ctrl #number options:

# Sample run
# Change directories below as required
# find /workspace/software/spark-2.0.0-bin-hadoop2.6/examples/src/main/scala/org/apache/spark/examples -name "*.scala" -exec grep -iH sparksession {} \; | awk -F':' '{print $1}' | sort -u > /root/runExamples
# change directories to . : For example if in runExamples we get a/example. we need to change it to a.example.
# get the data set up in HDFS at the required directory.
# run the following command
# for i in `cat runExamples `; do sparkExampleWrapper.sh $i ; done
run=$1
out=$run.log
cd /workspace/linux-4.15-rc1/OS_Overheads/
./control --clear-all
./control --reset-all-stats
dmesg -c
./control --add java
./control --add python
./control --ctrl 4
sh -c "spark-submit run-example $run"
./control --list-stats
dmesg -c > /workspace/sparkExamples/$out


