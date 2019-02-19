#/bin/bash


outfile="./FADCSettings.txt"
LeftROLfile="fadc_vxs_apex.c"
RightROLfile="roc20_tritium_list.c"

LeftPath="/adaqfs/home/adaq/fadc_mike/BlkRead/"
RightPath="/adaqfs/home/adaq/tritium_fadc/BlkRead/"

LeftROL="${LeftPath}""${LeftROLfile}"

RightROL="${RightPath}""${RightROLfile}"

echo ${LeftROL}
echo ${RightROL}

echo 
echo
if [ -e ${outfile} ]
then
	rm -v -f $outfile
fi

echo "Right arm">>$outfile
grep -n -r "#define FADC_NSA" ${RightROL}>> $outfile
grep -n -r "#define FADC_NSB" ${RightROL}>> $outfile
grep -n -r "#define FADC_WIN" ${RightROL}>> $outfile

echo "Left arm">>$outfile
grep -n -r "#define FADC_NSA" ${LeftROL} >> $outfile
grep -n -r "#define FADC_NSB" ${LeftROL} >> $outfile
grep -n -r "#define FADC_WIN" ${LeftROL} >> $outfile
