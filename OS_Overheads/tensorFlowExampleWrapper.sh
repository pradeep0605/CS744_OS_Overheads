#!/bin/bash
# [--add | --list-apps | --clear app_name | --clear-all | --ctrl #number | --add-stats | --clear-stats |
# --clear-all-stats | --reset-stats | --reset-all-stats | --list-stats]
#--ctrl #number options:
cd /workspace/linux-4.15-rc1/OS_Overheads/
./control --clear-all
./control --reset-all-stats
dmesg -c
./control --add sh 
./control --ctrl 4
cd /workspace/tensorflow/
run="python3.4 cartpole.py"
out=cartpole.py.log
sh -c "$run"
cd /workspace/linux-4.15-rc1/OS_Overheads/
./control --list-stats
dmesg -c > /workspace/tensorFlowExamples/$out
