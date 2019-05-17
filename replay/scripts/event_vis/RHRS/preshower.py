from vpython import *

#####################
## BUILD PRESHOWER ##
#####################

def build_preshower():

	# preshower block dimensions (from database)
	psdx = 0.10
	psdy = 0.35
	psdz = 0.1

	psrow = 24
	pscol = 2

	# x center of block at lowest x position  
	psxmin = -1.2 + psdx/2.
	# y center of block at lowest y position
	psymin = -0.35 + psdy/2.
	# z position of preshower (from database)
	psz = 3.5 + psdz/2.

	ps = []

	# need to draw lines on edges to distinguish blocks, real PITA
	psfront = []
	psback = []
	psleft = [] 
	psright = []

	# must loop columns first to preserve block numbering
	for i in range(pscol):
		for j in range(psrow):
			x = psxmin + j*psdx
			y = psymin + i*psdy
			z = psz
			ps.append(box(pos=vector(x,y,z), length=psdx, height=psdy, width=psdz, opacity = 1.0))		

			psfront.append(curve(color=color.black, radius = 0.005))
			psfront[i*psrow+j].append([vector(x-psdx/2.,y-psdy/2.,z-psdz/2.),
						vector(x+psdx/2.,y-psdy/2.,z-psdz/2.),
						vector(x+psdx/2.,y+psdy/2.,z-psdz/2.),
						vector(x-psdx/2.,y+psdy/2.,z-psdz/2.),
						vector(x-psdx/2.,y-psdy/2.,z-psdz/2.)])
			
			psback.append(curve(color=color.black, radius = 0.005))
			psback[i*psrow+j].append([vector(x-psdx/2.,y-psdy/2.,z+psdz/2.),
						vector(x+psdx/2.,y-psdy/2.,z+psdz/2.),
						vector(x+psdx/2.,y+psdy/2.,z+psdz/2.),
						vector(x-psdx/2.,y+psdy/2.,z+psdz/2.),
						vector(x-psdx/2.,y-psdy/2.,z+psdz/2.)])
			psleft.append(curve(color=color.black, radius = 0.005))
			psleft[i*psrow+j].append([vector(x-psdx/2.,y-psdy/2.,z+psdz/2.),
						vector(x+psdx/2.,y-psdy/2.,z+psdz/2.),
						vector(x+psdx/2.,y-psdy/2.,z-psdz/2.),
						vector(x-psdx/2.,y-psdy/2.,z-psdz/2.),
						vector(x-psdx/2.,y-psdy/2.,z+psdz/2.)])
			psright.append(curve(color=color.black, radius = 0.005))
			psright[i*psrow+j].append([vector(x-psdx/2.,y+psdy/2.,z+psdz/2.),
						vector(x+psdx/2.,y+psdy/2.,z+psdz/2.),
						vector(x+psdx/2.,y+psdy/2.,z-psdz/2.),
						vector(x-psdx/2.,y+psdy/2.,z-psdz/2.),
						vector(x-psdx/2.,y+psdy/2.,z+psdz/2.)])
				
	return ps
