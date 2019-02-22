#!/bin/bash

# eep runs

mkdir  log
mkdir  plots
mkdir  plots/LHRS
mkdir  plots/RHRS
counter=3061 
rhrs=1
while [ $counter -le 3188 ]
do
analyzer -b "check_TSscaler.C($counter)"> log/$counter.out
((counter++))
done

counter=93040 
while [ $counter -le 93148 ]
do
analyzer -b "check_TSscaler.C($counter,$rhrs)">> log/$counter_R.out
((counter++))
done

