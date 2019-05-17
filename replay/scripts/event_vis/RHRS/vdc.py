from vpython import *
import numpy as np

###############
## BUILD VDC ##
###############

def build_vdc():

	L = 2.188
	W = 0.288
	dUV = 0.026

	nwire = 368
	dwire = 0.0042426	

	# wires are at 45 degrees brah.
	# coordinate system is at 45 degrees brah. 
	# gonna need square root of 2 brah.
	sqrt2 = np.sqrt(2.0)

	u1 = []
	v1 = []
	u2 = []
	v2 = []

	## U1 and V1 ##

	pfirstwire = 0.77852
	zu = 0	
	zv = zu + dUV
	for i in range(nwire):
		pwire = pfirstwire - i*dwire
		xi = pwire*sqrt2 - W/2
		xf = xi + W
		yi = W/2
		yf = -W/2
		if xf > L/2:
			xf = L/2
			yf = pwire*sqrt2 - L/2
		if xi < -L/2:
			xi = -L/2
			yi = pwire*sqrt2 + L/2
		u1.append(curve(pos=[vector(xi,-yi,zu), vector(xf,-yf,zu)], radius=0.001, color=color.white))
		v1.append(curve(pos=[vector(xi,+yi,zv), vector(xf,+yf,zv)], radius=0.001, color=color.white))
		u1[i].rotate(angle=-np.pi/4, axis=vector(0,1,0))
		v1[i].rotate(angle=-np.pi/4, axis=vector(0,1,0))
		
	## U2 and V2 ##

	pfirstwire = 0.77852
	dx = (1.02793 - 0.77852)*sqrt2
	zu = 0.3327	
	zv = zu + dUV
	for i in range(nwire):
		pwire = pfirstwire - i*dwire
		xi = pwire*sqrt2 - W/2
		xf = xi + W
		yi = W/2
		yf = -W/2
		if xf > L/2:
			xf = L/2
			yf = pwire*sqrt2 - L/2
		if xi < -L/2:
			xi = -L/2
			yi = pwire*sqrt2 + L/2
		u2.append(curve(pos=[vector(xi+dx,-yi,zu), vector(xf+dx,-yf,zu)], radius=0.001, color=color.white))
		v2.append(curve(pos=[vector(xi+dx,+yi,zv), vector(xf+dx,+yf,zv)], radius=0.001, color=color.white))
		u2[i].rotate(angle=-np.pi/4, axis=vector(0,1,0))
		v2[i].rotate(angle=-np.pi/4, axis=vector(0,1,0))

	## Frames ##
	thick = 0.01
	thick_rel = thick/W
	frame_H = W + 2*thick
	frame_W = L + 2*thick
	rec = shapes.rectangle(width = frame_W, height = frame_H, thickness = thick_rel)
	c1 = [vector(0,0,0-thick),vector(0,0,dUV+thick)]
	c2 = [vector(0,0,0.3327*sqrt2-thick),vector(0,0,0.3327*sqrt2+thick+dUV)]
	frame1 = extrusion(path = c1, shape = rec, color = color.gray(0.5))	
	frame2 = extrusion(path = c2, shape = rec, color = color.gray(0.5))	
	frame1.rotate(angle=-np.pi/4, axis=vector(0,1,0))
	frame2.rotate(angle=-np.pi/4, axis=vector(0,1,0))

	for i in range(nwire):
		u1[i].visible=False;
		v1[i].visible=False;
		u2[i].visible=False;
		v2[i].visible=False;

	return u1, v1, u2, v2



