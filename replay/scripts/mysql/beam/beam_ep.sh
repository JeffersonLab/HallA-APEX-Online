#!/bin/bash

# eep runs
mkdir  log
mkdir  plots
mkdir  elist
counter=100030 #100030 
while [ $counter -le 100688 ]
do
analyzer -b -q "beamtrip_sql.C($counter)"> log/$counter.out
((counter++))
done
