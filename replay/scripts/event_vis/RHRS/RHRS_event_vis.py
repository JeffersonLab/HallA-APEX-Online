from vpython import *
import numpy as np
import sys
import os
import signal
import ROOT
from vdc import build_vdc
from preshower import build_preshower
from shower import build_shower

if len(sys.argv) != 2:
	print("Please specify a ROOT file")
	sys.exit()
rootfilename = str(sys.argv[1])

###############
## DRAW AXES ##
###############

# scene axes (transport coordinates)   
#arrow(pos=vector(0.,0.,0.), axis=vector(0.5,0.,0.), color=color.blue)
#arrow(pos=vector(0.,0.,0.), axis=vector(0.,0.5,0.), color=color.green)
#arrow(pos=vector(0.,0.,0.), axis=vector(0.,0.,0.5), color=color.red)

#####################
## BUILD DETECTORS ##
#####################

u1,v1,u2,v2 = build_vdc()
ALL_ACTIVE = True

ps = build_preshower()
sh = build_shower()

###############
## SET SCENE ##
###############

# make sure all sides are lit
scene.lights.append(distant_light(direction=vector(0.,0.,-1.),color=color.gray(0.5)))
scene.lights.append(distant_light(direction=vector(0.,-1.,0.),color=color.gray(0.5)))
scene.lights.append(distant_light(direction=vector(0.,+1.,0.),color=color.gray(0.5)))
scene.lights.append(distant_light(direction=vector(-1.,0.,0.),color=color.gray(0.5)))
scene.lights.append(distant_light(direction=vector(+1.,0.,0.),color=color.gray(0.5)))

scene.center = vector(0.,0.,0)

# optional: choose initial orientation of the camera
scene.up = vector(-1.,0.,1.)
scene.forward = vector(0.,0.,1.)

###############################################
## DEFINE FUNCTIONS TO DRAW AND CLEAR EVENTS ##
###############################################

# function to clear event from visualization
def clear_event(currentTracks):
	# delete tracks
	for i in range(len(currentTracks)):
		# always delete first element, as element (i+1)->(i) when (i) is deleted
		currentTracks[0].visible = False
		del currentTracks[0]
	# reset block colors
	for i in range(len(ps)):
		ps[i].color = color.white
	for i in range(len(sh)):
		sh[i].color = color.white
	for i in range(len(u1)):
		u1[i].color = color.white	
		v1[i].color = color.white	
		u2[i].color = color.white	
		v2[i].color = color.white	
		u1[i].visible = False
		v1[i].visible = False
		u2[i].visible = False
		v2[i].visible = False

def draw_track(ntr, xtr, ytr, thtr, phtr):
	track = []
	for i in range(ntr):
		track.append(curve(color=color.green, radius=0.005))
		track[i].append([vector(xtr[i]+thtr[i]*drift_b, ytr[i]+phtr[i]*drift_b, drift_b), vector(xtr[i]+thtr[i]*drift_f, ytr[i]+phtr[i]*drift_f, drift_f)])
	return track

def draw_event_cal(cal, mult, blk, adc):
	blockADC = [0 for i in range(mult)]
	blockNum = [-1 for i in range(mult)]
	for i in range(mult):
		blockNum[i] = int(blk[i])
		blockADC[i] = adc[blockNum[i]]
	minADC = min(blockADC)
	maxADC = max(blockADC)
	if maxADC-minADC > 0.:
		for i in range(mult):
			blockSat = 0.5 + (0.5/(maxADC - minADC))*(blockADC[i] - minADC)
			blockColor = color.hsv_to_rgb(vector(0.,blockSat,1.))
			cal[blockNum[i]].color = blockColor
	else:
		for i in range(mult):
			blockColor = color.hsv_to_rgb(vector(0.,0.75,1.))
			cal[blockNum[i]].color = blockColor

def draw_event_vdc(vp, clb, cle, clt, clw):
	if ALL_ACTIVE:
		nwire = len(clw)
		for i in range(nwire):
			w = int(clw[i])
			vp[w].visible = True
			vp[w].color = color.blue
	nclust = len(clb)
	for i in range(nclust):
		begwire = int(clb[i])
		endwire = int(cle[i])+1
		for w in range(begwire, endwire):
			vp[w].visible = True
			if clt[i] != 0:
				vp[w].color=color.red
			else:
				vp[w].color=color.yellow 


#########################
## ROOT FILE FUNCTIONS ##
#########################

def get_track(tree, eventNum):
	tree.GetEntry(eventNum)
	ntr = getattr(tree,"R.tr.n")
	xtr = getattr(tree, "R.tr.x")
	ytr = getattr(tree, "R.tr.y")
	thtr = getattr(tree, "R.tr.th")
	phtr = getattr(tree, "R.tr.ph")
	return ntr, xtr, ytr, thtr, phtr

def get_event_cal(cal, tree, eventNum):
	tree.GetEntry(eventNum)
	mult = getattr(tree, "R." + cal + ".mult")
	blk = getattr(tree, "R." + cal + ".nblk")
	adc = getattr(tree, "R." + cal + ".a_c")
	return mult, blk, adc

def get_event_energy(tree, eventNum):
	tree.GetEntry(eventNum)
	pse = getattr(tree, "R.ps.e")
	she = getattr(tree, "R.sh.e")
	ptr = getattr(tree, "R.tr.p")
	cer = getattr(tree, "R.cer.a_c")
	return pse, she, ptr, cer

def get_event_triggers(tree, eventNum):
	tree.GetEntry(eventNum)
	bit4 = getattr(tree, "DR.bit4")
	bit5 = getattr(tree, "DR.bit5")
	bit6 = getattr(tree, "DR.bit6")
	return bit4, bit5, bit6

def get_event_vdc(vp, tree, eventNum):
	tree.GetEntry(eventNum)
	b = getattr(tree, "R.vdc." + vp + ".clbeg")
	e = getattr(tree, "R.vdc." + vp + ".clend")
	t = getattr(tree, "R.vdc." + vp + ".cltrknum")
	w = getattr(tree, "R.vdc." + vp + ".wire")
	return b, e, t, w
	
#########################
## DRAW TRACK AND HITS ##
#########################

rootfile = ROOT.TFile.Open(rootfilename)
T = rootfile.Get("T")

evCuts=("DR.bit5==1"
	"&&R.tr.n==1")

evList = ROOT.TEventList("evList")
T.Draw(">>evList",evCuts)			

drift_f = 4.5
drift_b = -1.0

listIndex = 0

userinput = ''

while userinput != 'exit':

	evNum = evList.GetEntry(listIndex)

	fntr, fxtr, fytr, fthtr, fphtr = get_track(T, evNum)
	fpsmult, fpsblk, fpsadc = get_event_cal("ps", T, evNum)
	fshmult, fshblk, fshadc = get_event_cal("sh", T, evNum)
	fpse, fshe, fptr, fcer = get_event_energy(T, evNum)
	fbit4, fbit5, fbit6 = get_event_triggers(T, evNum)
	fu1b, fu1e, fu1t, fu1w = get_event_vdc("u1", T, evNum)
	fv1b, fv1e, fv1t, fv1w = get_event_vdc("v1", T, evNum)
	fu2b, fu2e, fu2t, fu2w = get_event_vdc("u2", T, evNum)
	fv2b, fv2e, fv2t, fv2w = get_event_vdc("v2", T, evNum)
	print("Event " + str(evNum) + " has " + str(int(fntr)) + " track[s]")

	fTracks = draw_track(int(fntr), fxtr, fytr, fthtr, fphtr)

	draw_event_cal(ps, int(fpsmult), fpsblk, fpsadc) 

	draw_event_cal(sh, int(fshmult), fshblk, fshadc) 

	draw_event_vdc(u1, fu1b, fu1e, fu1t, fu1w)
	draw_event_vdc(v1, fv1b, fv1e, fv1t, fv1w)
	draw_event_vdc(u2, fu2b, fu2e, fu2t, fu2w)
	draw_event_vdc(v2, fv2b, fv2e, fv2t, fv2w)

	trstr = ""
	for i in range(int(fntr)):
		trstr += "Track %i p = %f GeV\t" % (i+1, fptr[i])

	scene.caption = ("Preshower E = %f GeV\t" % (fpse/1000.) + 
			"    Shower E = %f GeV\t" % (fshe/1000.) +
			trstr)
								
	userinput = input("Press return to draw next event\nEnter 'exit' to quit\n")
	
	listIndex+=1
	clear_event(fTracks)

os.kill(os.getpid(),signal.SIGKILL)

