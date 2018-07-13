## Code to deal with beam trip: select events from every stable beam current values and calculate corresponding charge.

* This code loop over the fast scaler output (evLeft/evRight) to identify stable beam period (by default after every beamtrip will throw away the first 5 secs of stable beam for gas cells).
* For each beam on current (identified by TSpectrum), it calculats the mean current, charge, and live time (for production trigger 2 or 5), then stores results at SQL database triton-work/{experiment}analysis.
* It also generate an event list elist/bcm{runnumber}_{current_id}_{cut on how many seconds for stable beam}.root. Beam current, charge, and livetime are stored in the rootfile as TVectors, please read the last part of main code to see how to extract informations.

### For ALL your later analysis, please load the corresponding eventlist at the beginning of your scripts to get rid of events from  bad beam. See example.C.

```
analyzer [0] .L rootalias.h 
analyzer [1] GetNCurrents(1555)
          current_id             current              charge
                   0                22.9             4588.58
                   1                20.4             5312.21
                   2                15.6             3729.31
                   3                10.1             2885.41
                   4                 4.9                1618
                   5                 2.1              424.84
analyzer [3] AnalysisInfo ana_info=GetAnalysisInfo(1213,0);
          current_id             current              charge
                   0                20.4             23263.9
analyzer [4] ana_info.trigger
(TString &) "DL.bit2"[7]
analyzer [5] ana_info.livetime
(Double_t) 0.951973
analyzer [6] ana_info.charge
(Double_t) 23263.9
```
