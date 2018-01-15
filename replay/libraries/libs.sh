#!/bin/bash
currentdir=`pwd`

for dir in */; do  echo "$dir";
echo " "
echo " "
echo "Changing to $dir, and compiling the library!"
cd $currentdir/$dir
echo " "

make realclean
make clean

echo " "

echo " "
make
done
echo " "

echo " "
echo "Done compiling the library! Changing back to the libraries main dir."
chmod -Rf 777 libraries/ 
cd $currentdir





