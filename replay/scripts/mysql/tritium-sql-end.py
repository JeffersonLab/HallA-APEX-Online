#!/usr/bin/python

#######################################################
# A python script to insert run information into the
#   tritium MySQL run list.
# Author: Tyler Hague
# Created: 28 Nov 2017
# 
# modified to match new format - shujie, 2018.5
#######################################################

import sys
import os
import string
import MySQLdb
import t2log
from datetime import datetime as dt
#Uncomment the current experiment so that the correct table is filled.
#EXP = "TEST"
#EXP = "PRECOMMISSIONING"
#EXP = "COMMISSIONING"
#EXP = "MARATHON"
EXP = "SRC"
#EXP = "EP"
#EXP = "EK"

DEBUG =False
#print(sys.argv[1],sys.argv[1]=="right")
if len(sys.argv)>=2:
    if sys.argv[1]=="right":
        right_arm = True
    elif sys.argv[1]=="left":
        right_arm = False
    else:
        print 'The second argument must be \'right\' or \'left\''
        sys.exit(1)
    if len(sys.argv)==4:
        DEBUG = True
else:
    print 'Please specify if the code is being run for \'right\' or \'left\''
    sys.exit(1)

#Read in the run number from the rcRunNumber file

if len(sys.argv)>2:
    runnum = sys.argv[2]
else:
    try:
        if right_arm:
            runnum_file = open("/adaqfs/home/adaq/datafile/rcRunNumberR","r")
        else:
            runnum_file = open("/adaqfs/home/adaq/datafile/rcRunNumber","r")
            runnum = runnum_file.readline()
            runnum = runnum.rstrip() #Removes \n end of line character and any trailing whitespace. There shouldn't be any in this case, but just in case
            runnum_file.close()
    except IOError:
        print 'Run Number not found by the MySQL script. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'
        sys.exit(1) #Exit. Run number is the primary key, so an insert cannot be made without it


# get the log entry number for start/end of run 
print t2log.read_log(runnum,'end',0)

#runnum = sys.argv[2]
#######################################################
# Try connecting to the database. Exit if fail.
#######################################################
if not DEBUG:
    try:
        db = MySQLdb.connect(host='halladb.jlab.org', user='triton', passwd='cebaF111', db="triton")
    except MySQLdb.Error:
        print 'Could not connect to database. Please ensure that the paper runlist is kept up-to-date. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'
        sys.exit(1)
    try:
        db2 = MySQLdb.connect(host='halladb.jlab.org', user='triton-user', passwd='3He3Hdata', db="triton-work")
    except MySQLdb.Error:
        print 'Could not connect to database. Please ensure that the paper runlist is kept up-to-date. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'
        sys.exit(1)


#######################################################
# Ensure that the run number exists in the table
#   We are updating the entry.
#######################################################
if not DEBUG:
    cursor = db.cursor()
    cursor2 = db2.cursor()
    
    unique_test = "SELECT run_number FROM " + EXP + "runlist where run_number=" + runnum
    
    #Get number of entries with the current run number as a uniqueness test
    #Exit if not unique
    
    cursor.execute(unique_test)
    Evts = cursor.fetchall()
    evtAll = [Evt[0] for Evt in Evts]
    nEvtAll = len(evtAll)

    if nEvtAll==0:
        print 'This run number does not exist in the run_list. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'
        sys.exit(1)
    
    cursor2.execute(unique_test)
    Evts = cursor2.fetchall()
    evtAll = [Evt[0] for Evt in Evts]
    nEvtAll = len(evtAll)

    if nEvtAll==0:
        print 'This run number does not exist in the run_list. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'
        sys.exit(1)

#######################################################
# Extract end of run info to update the database
#######################################################

#Extracting end of run comments
try:
    if right_arm:
        comment_file = open("/adaqfs/home/adaq/scripts/.runendR.comments","r")
    else:
        comment_file = open("/adaqfs/home/adaq/scripts/.runendL.comments","r")
    found = False
    end_comment = ''
    for line in comment_file:
        if not found:
            i = 0
            while i<(len(line)-13) and not found:
                if line[i:i+13]=="comment_text=":
                    found = True
                    end_comment = line[i+13:].rstrip() + ' '
                i += 1
        else:
            end_comment += line.rstrip() + ' '
    end_comment = end_comment.rstrip()
    comment_file.close()
except IOError:
    print 'The end of run comment file seems to be missing. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'

#Extract time, events, trigger totals, and charge
triggers = ['NULL' for _ in range(8)]
charge = '-0.01'
time = 'NULL'
events = 'NULL'
try:
    if right_arm:
	logfile   = "/adaqfs/home/adaq/epics/runfiles_tritium_R/halog_com_" + runnum + ".epics"
        halog_com = open(logfile,"rb")
    else:
	logfile   = "/adaqfs/home/adaq/epics/runfiles_tritium_L/halog_com_" + runnum + ".epics" 
        halog_com = open(logfile,"rb")
    found_triggers = False
    found_charge = False
#    print halog_com.read()
    for line in halog_com:

	if line.startswith("EVENTS   : "):
            events = ''
            events = line.split(":")[1]
	elif line.startswith("TIME     : "):
            #i = 11
	    time = line.split(":")[1].split()[0]
	    #print line.split(":")[1].split(), time
        elif line.startswith("TRIGGER TOTALS:"):
            found_triggers = True
        elif found_triggers:
	    #print line
            i = -1
            fill = False
            for c in line:
                if c==':':
                    i += 1
                    fill = True
                    triggers[i] = ''
                elif c.isdigit() and fill:
                    triggers[i] += c
                elif c=='-' and fill:
                    triggers[i] = 'NULL'
                    fill = False
                elif (c==' ' and not triggers[i]=='') or (not c.isdigit() and not c==' '):
                    fill = False
            found_triggers = False
        elif line.startswith("APPROXIMATE BCM CHARGES"):
            found_charge = True
        elif found_charge:
            i=int(line.find("Dnew"))
           # print line.split("  ")[1] [5:-2]
            charge=line.split("  ")[1] [5:-2]#dnew  #charge=line[i+6:i+15]
	    found_charge = False
#        elif line.startswith("End-of-run EPICS"):
#	    print line
#	    print "--"
#	    endtime = dt.strptime(line.split(" at ")[1],'%a %b %d %H:%M:%S %Z %Y')
#	    endtime = endtime.strftime('%Y-%m-%d %H:%M:%S')

            break
#            i = 0
#            fill = False
#            while i<len(line):
#                if line[i:i+4] == 'Unew:':
#                    i=i+4
#                    fill = True
#                    charge = ''
                    
#                elif (line[i].isdigit() or line[i]=='.') and fill:
#                    charge += line[i]
#                elif line[i]=='-' and fill:
#                    charge = 'NULL'
#                    fill = False
#                else:
#                    fill = False
#                i += 1
#            found_charge = False
    halog_com.close()
except IOError:
    print logfile+' seems to be missing!.'

#######################################################
# Create and execute update statement
#######################################################

update_query = "UPDATE " + EXP + "runlist SET end_time=NOW(),end_comment=\"" + end_comment + "\", "
update_query += "event_count=" + events + ", "
update_query += "time_mins=" + time + ", "
update_query += "charge=" + charge + ", "
update_query += "T1_count=" + triggers[0] + ", "
update_query += "T2_count=" + triggers[1] + ", "
update_query += "T3_count=" + triggers[2] + ", "
update_query += "T4_count=" + triggers[3] + ", "
update_query += "T5_count=" + triggers[4] + ", "
update_query += "T6_count=" + triggers[5] + ", "
update_query += "T7_count=" + triggers[6] + ", "
update_query += "T8_count=" + triggers[7] + " "
update_query += "WHERE run_number=" + runnum

if not DEBUG:
    cursor.execute(update_query)
    cursor2.execute(update_query)
    print 'Successfully updated run',runnum,'to the MySQL run list! Have an awesome shift!'
else:
    print update_query

#print insert_query
