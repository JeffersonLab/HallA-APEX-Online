from vpython import *
import numpy as np
import sys
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
arrow(pos=vector(0.,0.,0.), axis=vector(0.5,0.,0.), color=color.blue)
arrow(pos=vector(0.,0.,0.), axis=vector(0.,0.5,0.), color=color.green)
arrow(pos=vector(0.,0.,0.), axis=vector(0.,0.,0.5), color=color.red)

#####################
## BUILD DETECTORS ##
#####################

vdc = build_vdc()
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
# center the camera between the VDC and the calorimter
scene.center = vector(0.,0.,2.)

# optional: choose initial orientation of the camera
#scene.forward = vector(0.,0.,1.)
#scene.up = vector(-1.,0.,0.)

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

# function to draw track[s]
def draw_track(ntr, xtr, ytr, thtr, phtr):
	track = []
	for i in range(ntr):
		track.append(curve(color=color.green, radius=0.005))
		track[i].append([vector(xtr[i],ytr[i],0.), vector(xtr[i]+thtr[i]*drift, ytr[i]+phtr[i]*drift, drift)])
	return track

# function to color preshower blocks based on ADC signal
def color_ps_blocks(psmult, psblk, psadc):
	blockADC = [0 for i in range(psmult)]
	blockNum = [-1 for i in range(psmult)]
	for i in range(psmult):
		blockNum[i] = int(psblk[i])
		blockADC[i] = psadc[blockNum[i]]
	minADC = min(blockADC)
	maxADC = max(blockADC)
	if maxADC-minADC > 0.:
		for i in range(psmult):
			blockSat = 0.5 + (0.5/(maxADC - minADC))*(blockADC[i] - minADC)
			blockColor = color.hsv_to_rgb(vector(0.,blockSat,1.))
			ps[blockNum[i]].color = blockColor
	else:
		for i in range(psmult):
			blockColor = color.hsv_to_rgb(vector(0.,0.75,1.))
			ps[blockNum[i]].color = blockColor

# function to color shower blocks based on ADC signal
def color_sh_blocks(shmult, shblk, shadc):
	blockADC = [0 for i in range(shmult)]
	blockNum = [-1 for i in range(shmult)]
	for i in range(shmult):
		blockNum[i] = int(shblk[i])
		blockADC[i] = shadc[blockNum[i]]
	minADC = min(blockADC)
	maxADC = max(blockADC)
	if maxADC-minADC > 0.:
		for i in range(shmult):
			blockSat = 0.5 + (0.5/(maxADC - minADC))*(blockADC[i] - minADC)
			blockColor = color.hsv_to_rgb(vector(0.,blockSat,1.))
			sh[blockNum[i]].color = blockColor
	else:
		for i in range(shmult):
			blockColor = color.hsv_to_rgb(vector(0.,0.75,1.))
			sh[blockNum[i]].color = blockColor


#########################
## ROOT FILE FUNCTIONS ##
#########################

def get_track_data(tree, eventNum):
	tree.GetEntry(eventNum)
	ntr = getattr(tree,"R.tr.n")
	xtr = getattr(tree, "R.tr.x")
	ytr = getattr(tree, "R.tr.y")
	thtr = getattr(tree, "R.tr.th")
	phtr = getattr(tree, "R.tr.ph")
	return ntr, xtr, ytr, thtr, phtr

def get_cluster_data(tree, eventNum):
	psmult = getattr(tree, "R.ps.mult")
	shmult = getattr(tree, "R.sh.mult")
	psblk = getattr(tree, "R.ps.nblk")
	shblk = getattr(tree, "R.sh.nblk")
	psadc = getattr(tree, "R.ps.a_c")
	shadc = getattr(tree, "R.sh.a_c")
	return psmult, shmult, psblk, shblk, psadc, shadc

def get_event_energy(tree, eventNum):
	pse = getattr(tree, "R.ps.e")
	she = getattr(tree, "R.sh.e")
	ptr = getattr(tree, "R.tr.p")
	return pse, she, ptr

#########################
## DRAW TRACK AND HITS ##
#########################

# get tree from ROOT file
rootfile = ROOT.TFile.Open(rootfilename)
T = rootfile.Get("T")

# how far to project the track
drift = 5

evNum = 0

while True:
	fntr = 0
	fpse = 0.0
	fshe = 0.0
	# get next event with at least one track
	# can add additional requirements to while loop to enforce more cuts
	while fntr == 0:  
		fntr, fxtr, fytr, fthtr, fphtr = get_track_data(T, evNum)
		fpsmult, fshmult, fpsblk, fshblk, fpsadc, fshadc = get_cluster_data(T, evNum)
		fpse, fshe, fptr = get_event_energy(T, evNum)
		evNum+=1
	print("Event " + str(evNum) + " has " + str(int(fntr)) + " track[s]")
	# draw track
	fTracks = draw_track(int(fntr), fxtr, fytr, fthtr, fphtr)
	# shade preshower blocks
	color_ps_blocks(int(fpsmult), fpsblk, fpsadc) 
	# shade shower blocks
	color_sh_blocks(int(fshmult), fshblk, fshadc) 
	# print event info
	print("Preshower E = %f GeV" % (fpse/1000.))
	print("Shower E = %f GeV" % (fshe/1000.))
	for i in range(int(fntr)):
		print("Track %i p = %f GeV" % (i, fptr[i]))
	input("Press return to draw next event")
	# clear event 
	clear_event(fTracks)


