#!/bin/bash

# run full skinny pass1 replay automatically for RHRS
# bash autopass1_R.sh <runnumber> 

# Shujie Li, Dec 2017
# Edited for pass1 autoreplay by R. Evan McClellan, Feb 2018


waittime=0
counter=0
RAWDIR=/adaq1/data1
#RAWDIR=/cache/halla/triton/raw
LOGDIR=/chafs1/work1/tritium/pass1/log
gstart=0   # start from which event
gtotal=-1 # replay how many events, -1 = full replay
ktrue=1
kfalse=0
thisrun=0
    

pc="$(uname -n)"
if [ $pc == "aonl1.jlab.org" ]; then  # to avoid repeating running
    echo ==========================================
    echo This script will run full replay automatically for RHRS
    echo Works only for recent runs that are stored at /adaq1/data1
    echo **If you want to terminate this program, do ctrl+z, kill % 
    echo ==========================================

    echo
    # to avoid repeating running. the script cannot be detected by pgrep if run with "source" command
    echo "Make sure you run the script with command 'bash':"
    select yn in "Yes" "No"; do
	case $yn in
	    Yes ) break;;
	    No ) exit;;
	esac
    done
    # check if is running already
    for pid in $(pgrep -f "bash autoreplay_R.sh"); do 
	if [ $pid != $$ ];then
	echo !!PID $pid ":Process is already running!!"
	exit 
	fi
    done

    runnum=`cat ~adaq/datafile/rcRunNumberR`
    echo **The current RHRS run number is $runnum
    if [ $# -eq 0 ];then
	echo "which run you want to start with?"
	read thisrun
    else
	thisrun=$1 # start from which run
    fi
    while [ $thisrun -gt $runnum ];do
	echo !!!RUN $thisrun does not exists. Please re-enter a smaller number:
	read thisrun
    done
    
    if [ $thisrun -lt 20000 ]; then
	echo "Please enter a RHRS run number!"
	exit
    fi
    runnum=`cat ~adaq/datafile/rcRunNumberR`
    echo **The current RHRS run number is $runnum
    echo "==Will start full replay from run " $thisrun
    

    # Check whether the raw data is ready
    # while [ $waittime -lt 144 ]; do  # if no new datafile for 24 hours, stop
    while [ $thisrun -le $runnum ]; do
	if [ $thisrun -lt $runnum ]; then
	
	    if [[ $(find ${RAWDIR}/triton_${thisrun}.dat.0 -type f -size +10000000c 2>/dev/null) ]]; then  # require rawdata > 10 Mbytes
		echo  ==Found ${RAWDIR}/triton_${thisrun}.dat.0
		if [ -e /chafs1/work1/tritium/pass1/tritium_${thisrun}.root ]; then
		    echo !!Can not overwrite /chafs1/work1/tritium/pass1/tritium_${thisrun}.root
		
		else 
		    echo Start analyzing
		    analyzer -q "./replay_tritium.C($thisrun,$gtotal,$gstart,$kfalse,$kfalse,$kfalse,$ktrue)"  >> ${LOGDIR}/${thisrun}.log
		    echo RUN $thisrun is analyzed
    		   
		fi
		
	    else 
		echo ${RAWDIR}/triton_${thisrun}.dat.0 less than 10 Mb. Will skip
		echo  ${RAWDIR}/triton_${thisrun}.dat.0 is skipped >> ${LOGDIR}/${thisrun}.log
	    fi
	    waittime=0
	    let thisrun=thisrun+1
	else
	    echo Run ${thisrun} is not completed.  Will check again after 10 minutes.
	    if [ $(($waittime % 5)) -eq 0 ]; then
		echo **If you want to terminate this program, do ctrl+z, kill %
	    fi
	    sleep 10m #wait for 10 minutes
	    waittime=$(($waittime + 1))
	    if [ $waittime -gt 144 ]; then
		echo ====no new datafile in the past 24 hours, STOP========
		exit
	    fi
	fi
#    let counter=counter+1
	#    echo $counter
    runnum=`cat ~adaq/datafile/rcRunNumberR`
    done
 
else
    echo !!!Please run this script on aonl1
    exit

fi


