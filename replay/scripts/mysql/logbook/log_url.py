import t2log as log
import sys
runnum = sys.argv[1]

a,b=log.read_log(runnum,'start')
a,b=log.read_log(runnum,'end'  )
log.read_online_url(runnum)