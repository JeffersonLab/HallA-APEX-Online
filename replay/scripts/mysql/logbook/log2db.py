#Updates start/end run comments and HV info to SQL runlist from log entry.
# Shujie Li, 2018.6

import urllib2, json
import subprocess
import sys
import requests # python3 preferred
import mysql.connector
from mysql.connector import errorcode
import t2log as log
# -----------------
#  please modify this.
db_user= 'triton-user'
db_pswd= '3He3Hdata'
db_name= 'triton-work'
db_host= 'halladb'

runnum = sys.argv[1]

if float(runnum)<1000:
  table="2017fall"
elif float(runnum)<3000:
  table="MARATHONrunlist"
elif float(runnum)<10000:
  table="SRCrunlist"
elif float(runnum)<90699:
  table="2017fall"
elif float(runnum)<93000:
  table="MARATHONrunlist"
elif float(runnum)<100000:
  table="SRCrunlist"
else:
  table="EPrunlist"




start_time,fstart  =log.read_log(runnum,'start')
end_time,fend      =log.read_log(runnum,'end'  )

#  
start_comment =  log.read_var(fstart,'comment_text',"=")
end_comment   =  log.read_var(fend,'End of Run Comment')
run_type      =  log.read_var(fstart,'Run_type',"=")
target        =  log.read_var(fstart,'target_type',"=") # whatever shift worker selected
Ebeam         =  log.read_var(fstart,'Tiefenbach 6GeV Beam energy')


counts        =  log.read_var(fend,'EVENTS')
# time_mins     =  log.read_var(fend,'TIME')
# time_mins     =  time_mins.split()[0]


if float(runnum)<100000:
  if float(runnum)<90000:
    # print log.read_hv(fend,'PRL2')
    angle       =  log.read_var(fstart,'HacL_CalcAngle')
    momentum    =  log.read_var(fstart,'Left Arm Q1 momentum')
    # print log.read_hv(fend,'Shower')
  else: 
    angle       =  log.read_var(fstart,'HacR_CalcAngle')
    momentum    =  log.read_var(fstart,'Right Arm Q1 momentum')
  
  create_entry   = ( "insert into `"+table+"`("  \
                    +"`run_number`,"\
                    +"`run_type`,"\
                    +"`start_time`,"\
                    +"`end_time`," \
                    +"`time_mins`,"  \
                    +"`target`,"  \
                    +"`beam_energy`,"\
                    +"`momentum`," \
                    +"`angle`,"\
                    +"`event_count`," \
                    +"`note`," \
                    +"`comment`,"\
                    +"`end_comment`," \
                    +"`modify_time`" \
                    +") values ("\
                    +runnum +","\
                    +"'"+run_type+"',"\
                    +"'"+start_time+"',"\
                    +"'"+end_time+"',"\
                    +'0'+","\
                    +"'"+target+"',"\
                    +Ebeam+","\
                    +momentum+","\
                    +angle+","\
                    +counts+","\
                    +"'created from logbook',"
                    +"'"+start_comment+"',"\
                    +"'"+end_comment+"',"+"now())"
                    )
else:
  angleL       =  log.read_var(fstart,'HacL_CalcAngle')
  momentumL    =  log.read_var(fstart,'Left Arm Q1 momentum')
  angleR       =  log.read_var(fstart,'HacR_CalcAngle')
  momentumR    =  log.read_var(fstart,'Right Arm Q1 momentum')

  create_entry   = ( "insert into `"+table+"`("  \
                    +"`run_number`,"\
                    +"`run_type`,"\
                    +"`start_time`,"\
                    +"`end_time`," \
                    +"`time_mins`,"  \
                    +"`target`,"  \
                    +"`beam_energy`,"\
                    +"`momentumL`," \
                    +"`momentumR`," \
                    +"`angleL`,"\
                    +"`angleR`,"\
                    +"`event_count`," \
                    +"`note`," \
                    +"`comment`,"\
                    +"`end_comment`," \
                    +"`modify_time`" \
                    +") values ("\
                    +runnum +","\
                    +"'"+run_type+"',"\
                    +"'"+start_time+"',"\
                    +"'"+end_time+"',"\
                    +'0'+","\
                    +"'"+target+"',"\
                    +Ebeam+","\
                    +momentumL+","\
                    +momentumR+","\
                    +angleL+","\
                    +angleR+","\
                    +counts+","\
                    +"'created from logbook',"
                    +"'"+start_comment+"',"\
                    +"'"+end_comment+"',"+"now())"
                    )

update_entry   = ( "update `"+table+"` set "  \
+"`start_time` ='"+start_time+"',"\
+"`end_time` ='"+end_time+"'," \
# +"`time_mins` ='"+time_mins+"',"  \
+"`comment` ='"+start_comment+"',"\
+"`end_comment` ='"+end_comment+"'," \
+"`modify_time`=now() where `run_number`=" + runnum \
)

print "good"
try:
    cnx = mysql.connector.connect(user=db_user,host=db_host,
                                database=db_name, password=db_pswd)
    cursor = cnx.cursor(buffered=True)
    cursor.execute("select * from "+table+" where run_number="+runnum)
    if cursor.rowcount == 0:
      print 'run '+runnum+' is not in '+table + ', will create a new entry from logbook'
      command_txt = create_entry
    else:
      print 'Will update run '+runnum
      command_txt = update_entry

except mysql.connector.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    print("Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    print("Database does not exist")
  else:
    print(err)

cursor.execute(command_txt)

#--------------------
# fill HV table

# create table cerL( run_number int(10) unsigned not null,pmt_id smallint(6) not null,pedestal float(8,3) default null,gain float(8,3) default null,primary key (run_number,pmt_id));

#create table R_PreShower ( run_number int(8) unsigned not null, id varchar(6) not null, hv float(10,3) not null default 1, primary key (run_number,id));
def fill_hv(arm,detector):
  index,set_hv,log.read_hv,current= get_hv(fend, detector)
  hv_table = arm+"_"+detector
  cursor.executemany("INSERT INTO `"+hv_table+ "` (`run_number`,`id`,`hv`) VALUES ('"+runnum+"',%s,%s)  ON DUPLICATE KEY UPDATE `hv`=values(`hv`)", [(x,y) for (x,y) in zip(index,log.read_hv)])
  return -1


if float(runnum)<90000:
  arm='L'
  fill_hv(arm,'PRL1')
  fill_hv(arm,'PRL2')

else:
  arm='R'
  fill_hv(arm,'PreShower')
  fill_hv(arm,'Shower')


fill_hv(arm,'Cherenkov')
fill_hv(arm,'VDC')
fill_hv(arm,'S0')
fill_hv(arm,'S2m')


cnx.commit()
cursor.close()
cnx.close()


