#!/bin/bash
# [--add | --list-apps | --clear app_name | --clear-all | --ctrl #number | --add-stats | --clear-stats |
# --clear-all-stats | --reset-stats | --reset-all-stats | --list-stats]
#--ctrl #number options:
cd /workspace/linux-4.15-rc1/OS_Overheads/
./control --clear-all
./control --reset-all-stats
dmesg -c
./control --add java
./control --add python
./control --ctrl 4
cd /workspace/flink-1.3.2/scripts/
run=PartCQuestion1-1.sh
out=$run.log
sh $run
cd /workspace/linux-4.15-rc1/OS_Overheads/
./control --list-stats
dmesg -c > /workspace/flinkExamples/$out

cd /workspace/linux-4.15-rc1/OS_Overheads/
./control --clear-all
./control --reset-all-stats
dmesg -c
./control --add java
./control --add python
./control --ctrl 4
cd /workspace/flink-1.3.2/scripts/
run=PartCQuestion1-2.sh
out=$run.log
sh $run
cd /workspace/linux-4.15-rc1/OS_Overheads/
./control --list-stats
dmesg -c > /workspace/flinkExamples/$out

