#import urllib3, json
import subprocess
import sys
import requests # python3 preferred
import MySQLdb
#import mysql.connector
#from mysql.connector import errorcode
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
elif float(runnum)<101000:
  table="EPrunlist"
else:
  table="EKrunlist"

flog = 0
if len(sys.argv)>2:
  flog = 1

# insert loglink to database table LogBook

start_log   = log.read_log(runnum,'start',0);
end_log     = log.read_log(runnum,'end'  ,0);
online_plot = log.read_online_url(runnum, 0);

try:
  db = MySQLdb.connect(host=db_host, user=db_user, passwd=db_pswd, db=db_name)
except MySQLdb.Error:
  print 'Could not connect to database', db_name, '!'
  sys.exit(1)
  
cursor = db.cursor()
query  = "insert into LogBook ( run_number, run_start, run_end, online_plot) values ("+runnum+",' "+ start_log+"',' "+end_log+"','"+online_plot+"')  on duplicate key update run_number=values(run_number), run_start=values(run_start), run_end=values(run_end), online_plot = values(online_plot);"

#print query
cursor.execute(query)
#result = cursor.fetchall()

if end_log=='0' or flog==1:
  exit()
  
#start_time,fstart,dummy  =log.read_log(runnum,'start')
end_time,fend,dummy      =log.read_log(runnum,'end')

#  
#start_comment =  log.read_var(fstart,'comment_text',"=")
end_comment   =  log.read_var(fend,'End of Run Comment')
#run_type      =  log.read_var(fstart,'Run_type',"=")
#target        =  log.read_var(fstart,'target_type',"=") # whatever shift worker selected
#Ebeam         =  log.read_var(fstart,'Tiefenbach 6GeV Beam energy')

#print(fend)
counts        =  log.read_var(fend,'EVENTS')
time_mins     =  log.read_var(fend,'TIME')
time_mins     =  time_mins.split()[0]
#print(counts,time_mins)
#exit()

#print(update_entry)
cursor.execute("select * from "+table+" where run_number="+runnum)
if len(cursor.fetchall())==0:
  print('run '+runnum+' is not in '+table + ', will create a new entry from logbook')
  start_time,fstart,dummy  =log.read_log(runnum,'start')
  start_comment =  log.read_var(fstart,'comment_text',"=")

  run_type      =  log.read_var(fstart,'Run_type',"=")
  target        =  log.read_var(fstart,'target_type',"=") # whatever shift worker selected
  Ebeam         =  log.read_var(fstart,'Tiefenbach 6GeV Beam energy')

  
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
  cursor.execute(create_entry)


update_query   = ( "update `"+table+"` set "  \
#+"`start_time` ='"+start_time+"'," \
+"`end_time`   ='"+end_time+  "'," \
+"`time_mins`  ='"+time_mins+ "'," \
+"`event_count`='"+counts+    "'," \
#+"`comment`    ='"+start_comment+"',"\
+"`end_comment`='"+end_comment+"',"  \
+"`modify_time`=now() where `run_number`=" + runnum \
)

#print update_query
print('Will update run '+runnum+' into '+table)
cursor.execute(update_query)

#--------------------
# fill HV table

# create table cerL( run_number int(10) unsigned not null,pmt_id smallint(6) not null,pedestal float(8,3) default null,gain float(8,3) default null,primary key (run_number,pmt_id));

#create table R_PreShower ( run_number int(8) unsigned not null, id varchar(6) not null, hv float(10,3) not null default 1, primary key (run_number,id));
def fill_hv(logentry,arm,detector):
  index,set_hv,read_hv,current= log.get_hv(logentry, arm, detector)
  hv_table = arm+"_"+detector
  cursor.executemany("INSERT INTO `"+hv_table+ "` (`run_number`,`id`,`hv`) VALUES ('"+runnum+"',%s,%s)  ON DUPLICATE KEY UPDATE `hv`=values(`hv`)", [(x,y) for (x,y) in zip(index,read_hv)])
  return -1


if float(runnum)<90000:
  arm='L'
#  print(log.get_hv(fend, arm, 'PRL1'))
  fill_hv(fend,arm,'PRL1')
  fill_hv(fend,arm,'PRL2')

else:
  arm='R'
  fill_hv(fend,arm,'PreShower')
  fill_hv(fend,arm,'Shower')


fill_hv(fend,arm,'Cherenkov')
fill_hv(fend,arm,'VDC')
fill_hv(fend,arm,'S0')
fill_hv(fend,arm,'S2m')




db.close()
cursor.close()
exit()
