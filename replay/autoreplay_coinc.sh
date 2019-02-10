#!/bin/bash

# run full replay automatically for LHRS
# bash autorepaly_coinc.sh <runnumber> 

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
    

pc="$(uname -n)"
if [ $pc == "aonl3.jlab.org" ]; then  # to avoid repeating running
    echo ==========================================
    echo This script will run full replay automatically for LHRS
    echo Works only for recent runs that are stored at /adaq1/data1
    echo **If you want to terminate this program, do ctrl+z, kill % 
    echo ==========================================

    echo
    # to avoid repeating running. the script cannot be detected by pgrep if run with "source" command
    echo "Make sure you run the script with commando 'bash':"
    select yn in "Yes" "No"; do
	case $yn in
	    Yes ) break;;
	    No ) exit;;
	esac
    done
    # check if is running already
   # for pid in $(pgrep -f "bash autoreplay_coinc.sh"); do 
   #	if [ $pid != $$ ];then
   #	echo !!PID $pid ":Process is already running!!"
   #	exit 
   #	fi
   # done

    runnum=`cat ~adaq/datafile/rcRunNumberR`
    echo **The current Coincidence run number is $runnum
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
    
    if [ $thisrun -lt 100000 ]; then
	echo "Please enter a e'p run number (>100000)!"
	exit
    fi
    runnum=`cat ~adaq/datafile/rcRunNumberR`
    echo **The current Coincidence run number is $runnum
    echo "==Will start full replay from run " $thisrun
    

    # Check whether the raw data is ready
    # while [ $waittime -lt 144 ]; do  # if no new datafile for 24 hours, stop
    while [ $thisrun -le $runnum ]; do
	if [ $thisrun -lt $runnum ]; then
	
	    if [[ $(find ${RAWDIR}/triton_${thisrun}.dat.0 -type f -size +10000000c 2>/dev/null) ]]; then  # require rawdata > 10 Mbytes
		echo  ==Found ${RAWDIR}/triton_${thisrun}.dat.0
		if [ -e ${t2root}/tritium_${thisrun}.root ]; then
		    
		    echo !! Can not Overwriting ${t2root}/tritium_${thisrun}.root
	           
           
		else 

  		    echo Start analyzing
		    analyzer -q "replay_coinc_new.C($thisrun,$gtotal,$gstart,$kfalse,$kfalse,$kfalse,$ktrue)"  >> ${LOGDIR}/${thisrun}.log
		    echo RUN $thisrun is analyzed
		   
		   # running the wiki runlist script to auto add thisrun to the wiki runlist
		   cd scripts
		   ./wiki_runlist $thisrun
		   analyzer -q -b "sql_update.C($thisrun)" >> ${LOGDIR}/${thisrun}_info.log
		   cd ..
		fi
		
	    else 
		echo ${RAWDIR}/triton_${thisrun}.dat.0 less than 10 Mb. Will skip
		echo  ${RAWDIR}/triton_${thisrun}.dat.0 is skipped >> ${LOGDIR}/${thisrun}.log
	    fi
	 #   waittime=0
	    let thisrun=thisrun+1
            waittime=2
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
    echo !!!Please run this script on aonl3
    exit

fi


