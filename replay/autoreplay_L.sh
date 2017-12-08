#!/bin/bash

# run full replay automatically for RHRS
# only work for recent runs that are stored on /adaq1/data1/
# will go run by run. will replay run n only if run n+1 exists 
# bash autorepaly.sh <runnumber> 

# Shujie Li, Dec 2017


waittime=0
counter=0
RAWDIR=/adaq1/data1
LOGDIR=${t2root}/log
gstart=0   # start from which event
gtotal=-1 # replay how many events, -1 = full replay
ktrue=1
kfalse=0
thisrun=0
temp=0
    

pc="$(uname -n)"
if [ $pc == "aonl3.jlab.org" ]; then  # to avoid repeating running

    # to avoid repeating running. the script cannot be detected by pgrep if run with "source" command
    echo "Make sure you run the script with command 'bash':"
    select yn in "Yes" "No"; do
	case $yn in
	    Yes ) break;;
	    No ) exit;;
	esac
    done
    # check if is running already
    for pid in $(pgrep -f "bash autoreplay_L.sh"); do 
	if [ $pid != $$ ];then
	echo PID $pid ":Process is already running"
	exit 
	fi
    done

    
    if [ $# -eq 0 ];then
	echo "which run you want to start with?"
	read thisrun
    else
	thisrun=$1 # start from which run
    fi
    
    if [ $thisrun -gt 20000 ]; then
	echo "Please enter a LHRS run number!"
	exit
    fi
    if [ -e ${RAWDIR}/triton_${thisrun}.dat.0 ]; then
	echo "Will start full replay from run " $thisrun
    else
	echo ${RAWDIR}/triton_${thisrun}.dat.0 "does not exist!!!"
	exit
    fi

    # Check whether the raw data is ready
    while [ $waittime -lt 144 ]; do  # if no new datafile for 24 hours, stop
	nextrun=$((${thisrun} + 1)) 
	if [ -e ${RAWDIR}/triton_${nextrun}.dat.0 ]; then
	    
	    if [[ $(find ${RAWDIR}/triton_${nextrun}.dat.0 -type f -size +10000000c 2>/dev/null) ]]; then  # require rawdata > 10 Mbytes
		echo  will analyze ${RAWDIR}/triton_${thisrun}.dat.0
		analyzer -q -d "replay_tritium.C($thisrun,$gtotal,$gstart,$kfalse,$kfalse,$kfalse,$ktrue)">> ${LOGDIR}/${thisrun}.log
		waittime=0
		
		echo  ${RAWDIR}/triton_${thisrun}.dat.0 is analyzed 
		
	    else 
		echo ${RAWDIR}/triton_${thisrun}.dat.0 less than 10 Mb. Will skip
		echo  ${RAWDIR}/triton_${thisrun}.dat.0 is skipped >> ${LOGDIR}/${thisrun}.log
		
	    fi
	    let thisrun=thisrun+1
	else
	    echo ${RAWDIR}/triton_${nextrun}.dat.0 does NOT exist. Maybe run ${thisrun} is still running?  Will check again after 10 minutes
	    sleep 10m #wait for 10 minutes
	    waittime=$(($waittime + 1))
	fi
#    let counter=counter+1
	#    echo $counter
    done
    echo ====no new datafile in the past 24 hours, STOP========
    exit

else
    echo Please run this code on aonl3
    exit

fi


