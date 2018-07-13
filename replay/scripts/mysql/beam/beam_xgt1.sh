#!/bin/bash

# xgt1 runs
mkdir  log
mkdir  plots
mkdir  elist
counter=3040
while [ $counter -le 3188 ]
do
analyzer -b -q "beamtrip_sql.C($counter)"> log/$counter.out
((counter++))
done
counter=93040
while [ $counter -le 93148 ]
do
analyzer -b -q "beamtrip_sql.C($counter)"> log/$counter.out
((counter++))
done
