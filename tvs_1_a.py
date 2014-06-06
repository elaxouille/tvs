#!/usr/bin/env python
# -*- coding: utf-8 -*- 


# TVS
# miccarr & elaxis

# Marge de mouvement non considéré
marge = 50

# Temps de réconfort (par tour)
decrementationpeur = 1
maxtouravantfinangoisse = 25

# Espacement entre les pixels à calculer
step = 4

print "\033[0;34m[RESPIRE]\033[0m \033[0;32mDémarrage de la kinect\033[0m (CTRL-C pour quitter)"
try:
	import freenect
except:
	print ( "\n\033[0;34m[RESPIRE]\033[0m \033[1;31mERREUR : Bibliothèque 'freenect' non trouvée.\033[0m" )
import time, sys, os, math
import subprocess

try :
	depth = freenect.sync_get_depth()[0]
except:
	print ( "\n\033[0;34m[RESPIRE]\033[0m \033[1;31mERREUR : Impossible de récupérer la profondeur.\033[0m La kinect est-elle branchée ?" )

# Taille de l'image recue du kinect
w = 640
h = 480

# Début de la simulation de respiration

prevFrame = [[0]*w]*h

result = 0

try:
	while 1:
		depth = freenect.sync_get_depth()[0]
		
		mouvement = 0

		for y in range(0, h-1, step) :
			for x in range(0, w-1, step) :
				if( depth[y][x] < prevFrame[y][x]-marge or depth[y][x] > prevFrame[y][x]+marge ):
					if(y < h/2):
						mouvement += y *2
					else:
						mouvement += (h - y) *2
					if(x < w/2):
						mouvement += x *2
					else:
						mouvement += (h - x) *2
		
		prevFrame = depth
		
		mouvement = ( mouvement * 100 / 3840000 ) - 3
		
		if (mouvement < 0):
			mouvement = 0
		elif mouvement > 100:
			mouvement = 100
			
		result += mouvement
		
		if result > 0 :
			result -= decrementationpeur
		if result > 100 + maxtouravantfinangoisse :
			result = 100 + maxtouravantfinangoisse
		print "Mouvement : \t", mouvement
		print "Result : \t", result
		
except (KeyboardInterrupt, SystemExit):
	freenect.Kill
	print "\n\033[0;34m[RESPIRE]\033[0m \033[0;32mArrêt de la kinect\033[0m"
except Exception, e:
	freenect.Kill
	e = sys.exc_info()[0]
	print ( "\n\033[0;34m[RESPIRE]\033[0m \033[1;31mERROR : %s\033[0m" % str(e) )
