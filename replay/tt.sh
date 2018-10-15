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
thisrun=100155
cd scripts
analyzer  "sql_update.C($thisrun)"  
# >> ${LOGDIR}/${thisrun}_info.log
cd ..	
