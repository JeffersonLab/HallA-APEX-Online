#!/bin/bash
#
initial=$1
final=$2
for i in `seq $initial $final`;
do
	root -b -l -q ./scripts/target_stability_macro.C\(3,\"\",${i}\) # T2 stability
	root -b -l -q ./scripts/target_stability_macro.C\(2,\"\",${i}\) # T6 stability
	root -b -l -q ./scripts/target_stability_macro.C\(1,\"\",${i}\) # integrated charge
done
