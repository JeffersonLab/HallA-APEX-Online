# Hall A Tritium Configuration Files

The replay code is currently being upgraded to work with analyzer 1.6 for the upcoming tritium run. In the replay directory the code is *replay_tritium.C*. In this code, *RIGHT_ARM_CONDITION* and *LEFT_ARM_CONDITION* are likely not correctly set, they are placeholders until we know how the run numbers will be divided. Adjust them as necessary to get them to work for the files you have.

Please test the code and email errors/problems to tjhague@jlab.org.

This repo currently contains currently contains stripped-down copies of the scaler subdirectories from the argon run. 

# How To Make Replay Code Work on local system

There are four lines of code that should be changed to make the replay code work on your machine:
1. rootlogon.C
    - Change *const char\* replay_dir_prefix* to reflect where your local replay code lives.
2. replay_tritium.C
    - Change the *#include* directive to reflect where *def_tritium.h* lives.
3. def_tritium.h
    - Change *const char\* REPLAY_DEF_PREFIX* to reflect where your local replay code lives.
    - Add your local raw data folder to the *static const char\* PATHS[]* array.

Once this is done you will need to recompile *ReplayCore64.C*. Copy the code below into a terminal in your replay directory to do this:
```
analyzer
.L ReplayCore64.C+
.q
```

You may also need to change *RAW_DATA_FORMAT* in *def_tritium.h* if you are using old data from another experiment.
