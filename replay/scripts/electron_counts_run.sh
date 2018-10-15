#!/bin/bash
# to run electron_counts.C

runnumL=`cat ~adaq/datafile/rcRunNumberL`
echo **The current LHRS run number is $runnumL

runnumR=`cat ~adaq/datafile/rcRunNumberR`
echo **The current RHRS run number is $runnumR
endrun=0
if [ $# -eq 0 ];then
    echo "which run you want to start with?"
    read thisrun
    echo "which run you want to end?"
    read endrun
#    echo "How many bins you want to have bw xbj=0 and 3? (30 is standard)"
#    read bins
else
    thisrun=$1 # start from which run

    if [ $thisrun -lt 10000 ];then
	endrun=$runnumL
    else
	endrun=$runnumR
    fi
fi


while [ $thisrun -le $endrun ]; do
   analyzer -b -q "electron_counts.C($thisrun,30)"
   analyzer -b -q "electron_counts.C($thisrun,150)"
    echo $thisrun
    thisrun=$(($thisrun + 1))
done
