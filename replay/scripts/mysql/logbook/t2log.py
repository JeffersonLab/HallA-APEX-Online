# A module of functions to read info from logbook
# Shujie Li, 2018.6


import urllib2, json
import subprocess
import sys
import requests
# -----------------
def read_log(runnum, flag):

  author  = "adaq"
  LOGBOOK = "HALOG"
  EFILE   =''

  prefix  =  'https://logbooks.jlab.org/entry/'  # Logbook entry URL prefix
  url     =  'https://logbooks.jlab.org/api/elog/entries'   # Base query url
  url     += "?book="+LOGBOOK                           # specify Logbook
  url     += '&limit=0'                                    # return all entries

  ## Constrain date (default is -180 days. ex. look back ~6 months)
     #$url .= '&startdate=-2 days';
     #$url .= '&enddate=-540 days';
  url       += '&startdate=2017-12-01'  ## 2015-01-01
     #$url .= '&enddate=2016-09-01';   ## 2016-09-01 00:00

  ## Constrain search to a Tag
  if flag   == 'start':
    url     += '&tag=StartOfRun'
    url     += "&title=Start_Run_"+runnum

  elif flag == 'end':
    url     += '&tag= EndOfRun'
    url     += "&title=End_of_Run_"+runnum


  ## Output fields
  url += '&field=attachments'
  url += '&field=lognumber&field=created&field=title'
  url += '&field=body&field=author&field=entrymakers'

  ## Append query fields
  # $url .= "&title=${mode}%20Start_Run_${run}";
  
  url += "&author="+author
  response = requests.get(url) #,auth=(log_user,log_pswd)) -- don't need this on a CUE level machine
  response = response.json()
  print flag+" of run log entry:", prefix+response['data']['entries'][0]['lognumber']
  content =  response['data']['entries'][0]['body']['content']
  content = content.split("\n")
  # print content
  if (flag=='start'):
    content = content[10:]
  # response = requests.get(url)
  # print(response.content)
  ctime = response['data']['entries'][0]['created']['string']+':00'
  return ctime,content

def read_online_url(runnum):

  author  = "adaq"
  LOGBOOK = "HALOG"
  EFILE   =''
  prefix  =  'https://logbooks.jlab.org/entry/'  # Logbook entry URL prefix

  url     =  'https://logbooks.jlab.org/api/elog/entries'   # Base query url
  url     += "?book="+LOGBOOK                           # specify Logbook
  url     += '&limit=0'                                    # return all entries

  ## Constrain date (default is -180 days. ex. look back ~6 months)
     #$url .= '&startdate=-2 days';
     #$url .= '&enddate=-540 days';
  url     += '&startdate=2017-12-01'  ## 2015-01-01
     #$url .= '&enddate=2016-09-01';   ## 2016-09-01 00:00

  ## Constrain search to a Tag
  url     += '&tag=Analysis'
  # url     += "&title= plot"
  # url     += "&title="+runnum
  url     += '&title=%20'+runnum+'%20'
  ## Output fields
  # url += '&field=attachments'
  url += '&field=lognumber&field=title'
  # url += '&field=body&field=author&field=entrymakers'

  ## Append query fields
  # $url .= "&title=${mode}%20Start_Run_${run}";

  try:
    response = requests.get(url).json()
    print response['data']['entries'][0]['title']+":",prefix+response['data']['entries'][0]['lognumber']
    # print response['data']['entries'][0]['attachments'][0]['url']
  except:
    print "error!"

  return 0


def read_var(f, name, delim=":"):
  # ss=f.split("\n")
  ss = f
  # print ss
  ii=0
  for line in ss:

    s = line.strip().split(delim)
    if name in s[0]:#.lower():
      # print s
      if len(s[1])>0:
        return delim.join(s[1:])
      else:
        return ss[ii+1]
    ii+=1

  print name+" not found"
  return "%d" %-1


def get_hv(f, detector):
  repeat2=['S2m','PRL1','PRL2','S2','PreShower','Shower'] # 2 rows
  repeat5=['Shower'] # 5 rows

  if detector in repeat5:
    repeat_rows=5
  elif detector in repeat2:
    repeat_rows=2
  else:
    repeat_rows=1

  try:
    flag=0
    lines = [x for x in f if x]
    i=0
    for line in lines:
      i=i+1
      if float(runnum)<90000:
        keyword = 'LHRS Detector High Voltage'
      else:
        keyword = 'RHRS Detector High Voltage'
      if keyword in line:
        block=lines[i:]
        i=0
        for ll in block:
          i+=1
          if ll==detector:
            index   = []
            set_hv  = []
            read_hv = []
            current = []

            for j in range(repeat_rows):
              index        += block[i+4*j].strip().split()
              set_hv       += block[i+4*j+1].strip().split()[3:]
              read_hv      += block[i+4*j+2].strip().split()[3:]
              current      += block[i+4*j+3].strip().split()[2:]

            break
        # print index, set_hv, read_hv, current

        return index, set_hv, read_hv, current

    return 'Error: missing hv info'
  except:
    return 'Unexpected HV value: check the content'
