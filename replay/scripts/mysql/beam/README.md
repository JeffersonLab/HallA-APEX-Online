## Code to deal with beamtrip

* This code loop over the fast scaler output (evLeft/evRight) to identify stable beam period (by default will throw away 5 secs of beam up for gas cells).
* For each stable run period, it calculated the mean current, charge, and live time (for production trigger 2 or 5), and stores results at SQL database triton-work/{experiment}analysis.
* It also generate an event list elist/bcm{runnumber}_{current_id}_{cut on how many seconds for stable beam}.root. Beam current, charge, and livetime are stored in the rootfile as TVectors, please read the last part of main code to see how to extract informations.

## For ALL your later analysis, please load the corresponding eventlist at the beginning of your scripts.