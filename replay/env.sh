#!/bin/bash
source /adaqfs/apps/ROOT/pro/bin/thisroot.sh
# Don't use a local analyzer, use the official one
#export ANALYZER=~/tritium/replay/analyzer-1.5.31
#export PATH=$ANALYZER:$PATH
#export LD_LIBRARY_PATH=$ANALYZER:$LD_LIBRARY_PATH 
export ANALYZER=/adaqfs/apps/analyzer
PATH=$ANALYZER/bin:$(unpath $PATH ~/tritium/replay/analyzer-1.5.31 /adaqfs/home/a-onl/tritium/replay/analyzer-1.5.31)
LD_LIBRARY_PATH=$ANALYZER/lib:$(unpath $LD_LIBRARY_PATH ~/tritium/replay/analyzer-1.5.31 /adaqfs/home/a-onl/tritium/replay/analyzer-1.5.31)
export PATH=$(repath $PATH)
export LD_LIBRARY_PATH=$(repath $LD_LIBRARY_PATH)

