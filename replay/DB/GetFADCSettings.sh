#/bin/bash


outfile="./FADCSettings.txt"
if [ -e ${outfile} ]
then
	rm -v -f $outfile
fi

echo "Right arm">>$outfile
grep -n -r "#define FADC_NSA" /adaqfs/home/adaq/tritium_fadc/BlkRead/roc20_tritium_list.c >> $outfile
grep -n -r "#define FADC_NSB" /adaqfs/home/adaq/tritium_fadc/BlkRead/roc20_tritium_list.c >> $outfile
grep -n -r "#define FADC_WIN" /adaqfs/home/adaq/tritium_fadc/BlkRead/roc20_tritium_list.c >> $outfile

echo "Left arm">>$outfile
grep -n -r "#define FADC_NSA" /adaqfs/home/adaq/fadc_mike/BlkRead/roc31_tritium_list.c >> $outfile
grep -n -r "#define FADC_NSB" /adaqfs/home/adaq/fadc_mike/BlkRead/roc31_tritium_list.c >> $outfile
grep -n -r "#define FADC_WIN" /adaqfs/home/adaq/fadc_mike/BlkRead/roc31_tritium_list.c >> $outfile
