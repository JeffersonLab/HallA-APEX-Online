#!/bin/bash
source /adaqfs/apps/ROOT/pro/bin/thisroot.sh

#####commented out due to issue in official analyzer installation
export ANALYZER=/adaqfs/apps/analyzer
PATH=$ANALYZER/bin:$(unpath $PATH ~/tritium/replay/analyzer-1.5.31 /adaqfs/home/a-onl/tritium/replay/analyzer-1.5.31)
LD_LIBRARY_PATH=$ANALYZER/lib:$(unpath $LD_LIBRARY_PATH ~/tritium/replay/analyzer-1.5.31 /adaqfs/home/a-onl/tritium/replay/analyzer-1.5.31)

##### temporary while official analyzer installation is being fixed
#export ANALYZER=/adaqfs/home/a-onl/tritium_work/analyzer
#PATH=$ANALYZER:$(unpath $PATH ~/tritium/replay/analyzer-1.5.31 /adaqfs/home/a-onl/tritium/replay/analyzer-1.5.31)
#LD_LIBRARY_PATH=$ANALYZER:$(unpath $LD_LIBRARY_PATH ~/tritium/replay/analyzer-1.5.31 /adaqfs/home/a-onl/tritium/replay/analyzer-1.5.31)


export PATH=$(repath $PATH)
export LD_LIBRARY_PATH=$(repath $LD_LIBRARY_PATH)

