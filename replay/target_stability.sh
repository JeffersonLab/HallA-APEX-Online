#!/bin/bash
#
initial=$1
final=$2
for i in `seq $initial $final`;
do
	root -b -l -q ./target_stability_macro.C\(7,\"\",${i}\) # T5/T2 avg value per run
	root -b -l -q ./target_stability_macro.C\(6,\"\",${i}\) # T6 avg value per run
	root -b -l -q ./target_stability_macro.C\(5,\"\",${i}\) # T5 avg value per run
	root -b -l -q ./target_stability_macro.C\(4,\"\",${i}\) # T2 avg value per run
	root -b -l -q ./target_stability_macro.C\(3,\"\",${i}\) # T2/uA stability
	root -b -l -q ./target_stability_macro.C\(2,\"\",${i}\) # T6/uA stability
	root -b -l -q ./target_stability_macro.C\(1,\"\",${i}\) # integrated charge
done
