from vpython import *

##################
## BUILD SHOWER ##
##################

def build_shower():

	shdx = 0.15
	shdy = 0.15
	shdz = 0.5

	shrow = 15
	shcol = 5

	# x center of block at lowest x position  
	shxmin = -1.15 + shdx/2.
	# y center of block at lowest y position
	shymin = -0.38 + shdy/2.
	# z position of preshower (from database)
	shz = 3.64 + shdz/2.

	sh = []

	# need to draw lines on edges to distinguish blocks. PITA.
	shfront = []
	shback = []
	shleft = [] 
	shright = []

	# must loop columns first to preserve block numbering
	for i in range(shcol):
		for j in range(shrow):
			x = shxmin + j*shdx
			y = shymin + i*shdy
			z = shz
			sh.append(box(pos=vector(x,y,z), length=shdx, height=shdy, width=shdz, opacity=1.0))		
			shfront.append(curve(color=color.black, radius = 0.005))
			shfront[i*shrow+j].append([vector(x-shdx/2.,y-shdy/2.,z-shdz/2.),
						vector(x+shdx/2.,y-shdy/2.,z-shdz/2.),
						vector(x+shdx/2.,y+shdy/2.,z-shdz/2.),
						vector(x-shdx/2.,y+shdy/2.,z-shdz/2.),
						vector(x-shdx/2.,y-shdy/2.,z-shdz/2.)])
			
			shback.append(curve(color=color.black, radius = 0.005))
			shback[i*shrow+j].append([vector(x-shdx/2.,y-shdy/2.,z+shdz/2.),
						vector(x+shdx/2.,y-shdy/2.,z+shdz/2.),
						vector(x+shdx/2.,y+shdy/2.,z+shdz/2.),
						vector(x-shdx/2.,y+shdy/2.,z+shdz/2.),
						vector(x-shdx/2.,y-shdy/2.,z+shdz/2.)])
			shleft.append(curve(color=color.black, radius = 0.005))
			shleft[i*shrow+j].append([vector(x-shdx/2.,y-shdy/2.,z+shdz/2.),
						vector(x+shdx/2.,y-shdy/2.,z+shdz/2.),
						vector(x+shdx/2.,y-shdy/2.,z-shdz/2.),
						vector(x-shdx/2.,y-shdy/2.,z-shdz/2.),
						vector(x-shdx/2.,y-shdy/2.,z+shdz/2.)])
			shright.append(curve(color=color.black, radius = 0.005))
			shright[i*shrow+j].append([vector(x-shdx/2.,y+shdy/2.,z+shdz/2.),
						vector(x+shdx/2.,y+shdy/2.,z+shdz/2.),
						vector(x+shdx/2.,y+shdy/2.,z-shdz/2.),
						vector(x-shdx/2.,y+shdy/2.,z-shdz/2.),
						vector(x-shdx/2.,y+shdy/2.,z+shdz/2.)])
		

	return sh
