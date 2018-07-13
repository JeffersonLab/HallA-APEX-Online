## Read Logbook Information

* tested with python2.7. Please modify the code to match your environment.
* required modules: json, requests, subprocess, urllib2, mysql.connector

### t2log.py:
A module of functions to read info from logbook

### log2db.py <runnum>:
Updates start/end run comments and HV info to SQL runlist from log entry.

### log_url.py <runnum>:
Returns urls for start/end run log and online plots.

