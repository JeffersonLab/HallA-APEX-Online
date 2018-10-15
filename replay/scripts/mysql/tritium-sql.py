#!/usr/bin/python

#######################################################
# A python script to insert run information into the
#   tritium MySQL run list.
# Author: Tyler Hague
# Created: 28 Nov 2017
#######################################################

import sys
import os
import string
import MySQLdb
import subprocess

#Uncomment the current experiment so that the correct table is filled.
#EXP = "TEST"
#EXP = "PRECOMMISSIONING"
#EXP = "COMMISSIONING"
#EXP = "MARATHON"
EXP = "SRC"
#EXP = "EP"
#EXP = "EK"

DEBUG = False

if len(sys.argv)==2:
    if sys.argv[1]=='right':
        right_arm = True
    elif sys.argv[1]=='left':
        right_arm = False
    else:
        print 'The second argument must be \'right\' or \'left\''
        sys.exit(1)
else:
    print 'Please specify if the code is being run for \'right\' or \'left\''
    sys.exit(1)

#Returns the value of the requested EPICS variable by interfacing with caget (a standalone script on adaq)
def caget(EPICS_var):
    caget_query = ['/adaqfs/home/adaq/scripts/caget','-t',EPICS_var]
    caget = subprocess.Popen(caget_query, stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE) #runs caget, first entry of caget_query is process. all following are args
    ret, err = caget.communicate()
    return ret.rstrip() #remove whitespace and \n characters

#Read in the run number from the rcRunNumber file
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
# Ensure that the run number does not exist in the
#   table already. Runnum is a unique key.
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
    
    if nEvtAll>0:
        print 'This run number is already in existence in the run_list. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'
        sys.exit(1)
    
    cursor2.execute(unique_test)
    Evts = cursor2.fetchall()
    evtAll = [Evt[0] for Evt in Evts]
    nEvtAll = len(evtAll)
    
    if nEvtAll>0:
        print 'This run number is already in existence in the run_list. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'
        sys.exit(1)

#######################################################
# Interact with the caget script to access
#   epics variables
#######################################################

#Get Run Type and Run Comments from the title file
runtype = "" #If something goes wrong with generating/reading this file default to blank
comment = ""
try:
    if right_arm:
        title_file = open("/adaqfs/home/adaq/scripts/RUN_INFO_R.TITLE_COL","r")
    else:
        title_file = open("/adaqfs/home/adaq/scripts/RUN_INFO_L.TITLE_COL","r")
    for line in title_file:
        if line.startswith("Run_type="):
            run_type = line[9:].rstrip()
        if line.startswith("comment_text="):
            comment = line[13:].rstrip()
    title_file.close()
except IOError:
    print 'Title file seems to be missing. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'

#Get variables from epics
target_encoder = float(caget('haBDSPOS.VAL'))
raster_x = caget('EHAR_LIXWidth')
raster_y = caget('EHAR_LIYWidth')
beam_energy = caget('MMSHLAE')

#Determine target from encoder value
target = ''
if abs(float(caget("haBDSPOS_0"))-target_encoder)<300.0:
    target = caget("haBDSSELECT.ZRST").rstrip()
elif abs(float(caget("haBDSPOS_1"))-target_encoder)<300.0:
    target = caget("haBDSSELECT.ONST").rstrip()
elif abs(float(caget("haBDSPOS_2"))-target_encoder)<300.0:
    target = caget("haBDSSELECT.TWST").rstrip()
elif abs(float(caget("haBDSPOS_3"))-target_encoder)<300.0:
    target = caget("haBDSSELECT.THST").rstrip()
elif abs(float(caget("haBDSPOS_4"))-target_encoder)<300.0:
    target = caget("haBDSSELECT.FRST").rstrip()
elif abs(float(caget("haBDSPOS_5"))-target_encoder)<300.0:
    target = caget("haBDSSELECT.FVST").rstrip()
elif abs(float(caget("haBDSPOS_6"))-target_encoder)<300.0:
    target = caget("haBDSSELECT.SXST").rstrip()
elif abs(float(caget("haBDSPOS_7"))-target_encoder)<300.0:
    target = caget("haBDSSELECT.SVST").rstrip()
elif abs(float(caget("haBDSPOS_8"))-target_encoder)<100.0:
    target = caget("haBDSSELECT.EIST").rstrip()
elif abs(float(caget("haBDSPOS_9"))-target_encoder)<100.0:
    target = caget("haBDSSELECT.NIST").rstrip()
elif abs(float(caget("haBDSPOS_10"))-target_encoder)<100.0:
    target = caget("haBDSSELECT.TEST").rstrip()
elif abs(float(caget("haBDSPOS_11"))-target_encoder)<100.0:
    target = caget("haBDSSELECT.ELST").rstrip()
elif abs(float(caget("haBDSPOS_12"))-target_encoder)<100.0:
    target = caget("haBDSSELECT.TVST").rstrip()
elif abs(float(caget("haBDSPOS_13"))-target_encoder)<100.0:
    target = caget("haBDSSELECT.TTST").rstrip()
elif abs(float(caget("haBDSPOS_14"))-target_encoder)<100.0:
    target = caget("haBDSSELECT.FTST").rstrip()
elif abs(float(caget("haBDSPOS_15"))-target_encoder)<100.0:
    target = caget("haBDSSELECT.FFST").rstrip()
if target=='':
    target = "Home (No Target)"
if target=="25 cm Dummy":
    target = "Dummy"
if target=="Carbon ":
    target = "Carbon"

if right_arm:
    momentum = caget('HacR_D1_P0rb')
else:
    momentum = caget('HacL_D1_P0rb')
if right_arm:
    angle = caget('HacR_CalcAngle')
else:
    angle = caget('HacL_CalcAngle')

#set columns to NULL if something goes wrong
prescaleT1 = "NULL"
prescaleT2 = "NULL"
prescaleT3 = "NULL"
prescaleT4 = "NULL"
prescaleT5 = "NULL"
prescaleT6 = "NULL"
prescaleT7 = "NULL"
prescaleT8 = "NULL"

#Read in prescale values from the generated prescale files
try:
    if right_arm:
        prescale_file = open("/adaqfs/home/adaq/prescale/prescaleR.dat","r")
    else:
        prescale_file = open("/adaqfs/home/adaq/prescale/prescaleL.dat","r")
    for line in prescale_file:
        if line.startswith("ps1="):
            length = len(line) #Just in case file is misformatted
            i=4
#in hindsight, this next part would do better as a function.
            while i<length and line[i].isdigit():
                if prescaleT1 == "NULL":  #overwrite NULL if we get this far
                    prescaleT1 = line[i]
                else:
                    prescaleT1 += line[i]
                i += 1
            while i<length and line[i]!='=': #Scan until the next equal sign. Doing this as opposed to i+=5 to make this safe for minor formatting changes in the prescale file
                i += 1
            i += 1
            while i<length and line[i].isdigit():
                if prescaleT2 == "NULL":  #overwrite NULL if we get this far
                    prescaleT2 = line[i]
                else:
                    prescaleT2 += line[i]
                i += 1
            while i<length and line[i]!='=': #Scan until the next equal sign. Doing this as opposed to i+=5 to make this safe for minor formatting changes in the prescale file
                i += 1
            i += 1
            while i<length and line[i].isdigit():
                if prescaleT3 == "NULL":  #overwrite NULL if we get this far
                    prescaleT3 = line[i]
                else:
                    prescaleT3 += line[i]
                i += 1
            while i<length and line[i]!='=': #Scan until the next equal sign. Doing this as opposed to i+=5 to make this safe for minor formatting changes in the prescale file
                i += 1
            i += 1
            while i<length and line[i].isdigit():
                if prescaleT4 == "NULL":  #overwrite NULL if we get this far
                    prescaleT4 = line[i]
                else:
                    prescaleT4 += line[i]
                i += 1
            while i<length and line[i]!='=': #Scan until the next equal sign. Doing this as opposed to i+=5 to make this safe for minor formatting changes in the prescale file
                i += 1
            i += 1
            while i<length and line[i].isdigit():
                if prescaleT5 == "NULL":  #overwrite NULL if we get this far
                    prescaleT5 = line[i]
                else:
                    prescaleT5 += line[i]
                i += 1
            while i<length and line[i]!='=': #Scan until the next equal sign. Doing this as opposed to i+=5 to make this safe for minor formatting changes in the prescale file
                i += 1
            i += 1
            while i<length and line[i].isdigit():
                if prescaleT6 == "NULL":  #overwrite NULL if we get this far
                    prescaleT6 = line[i]
                else:
                    prescaleT6 += line[i]
                i += 1
            while i<length and line[i]!='=': #Scan until the next equal sign. Doing this as opposed to i+=5 to make this safe for minor formatting changes in the prescale file
                i += 1
            i += 1
            while i<length and line[i].isdigit():
                if prescaleT7 == "NULL":  #overwrite NULL if we get this far
                    prescaleT7 = line[i]
                else:
                    prescaleT7 += line[i]
                i += 1
            while i<length and line[i]!='=': #Scan until the next equal sign. Doing this as opposed to i+=5 to make this safe for minor formatting changes in the prescale file
                i += 1
            i += 1
            while i<length and line[i].isdigit():
                if prescaleT8 == "NULL":  #overwrite NULL if we get this far
                    prescaleT8 = line[i]
                else:
                    prescaleT8 += line[i]
                i += 1
except IOError:
    print 'Prescale file seems to be missing. Please email Tyler Hague (tjhague@jlab.org) and include what run number this message appeared on.'

insert_query = "INSERT INTO " + EXP + "runlist("
insert_query += "run_number, "
insert_query += "run_type, "
insert_query += "start_time, "
insert_query += "target, "
insert_query += "raster_x, "
insert_query += "raster_y, "
insert_query += "beam_energy, "
insert_query += "momentum, "
insert_query += "angle, "
insert_query += "prescale_T1, "
insert_query += "prescale_T2, "
insert_query += "prescale_T3, "
insert_query += "prescale_T4, "
insert_query += "prescale_T5, "
insert_query += "prescale_T6, "
insert_query += "prescale_T7, "
insert_query += "prescale_T8, "
insert_query += "comment) "
insert_query += "VALUES( "
insert_query += runnum + ", "
insert_query += "\"" + run_type + "\", "
insert_query += "NOW(), "
insert_query += "\"" + target + "\", "
insert_query += raster_x + ", "
insert_query += raster_y + ", "
insert_query += beam_energy + ", "
insert_query += momentum + ", "
insert_query += angle + ", "
insert_query += prescaleT1 + ", "
insert_query += prescaleT2 + ", "
insert_query += prescaleT3 + ", "
insert_query += prescaleT4 + ", "
insert_query += prescaleT5 + ", "
insert_query += prescaleT6 + ", "
insert_query += prescaleT7 + ", "
insert_query += prescaleT8 + ", "
insert_query += "\"" + comment + "\") "


#######################################################
# Execute the insert statment
#######################################################
if not DEBUG:
    cursor.execute(insert_query)
    cursor2.execute(insert_query)
else:
    print insert_query

#print insert_query
print 'Successfully inserted into the MySQL run list! Have an awesome shift!'
