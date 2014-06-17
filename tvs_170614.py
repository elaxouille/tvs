#!/usr/bin/env python
# -*- coding: utf-8 -*- 


# TVS
# miccarr & elaxis
# 170614

# Marge de mouvement non considéré
marge = 50

# Temps de réconfort (par tour)
decrementationpeur = 6
maxtouravantfinangoisse = 12

# Espacement entre les pixels à calculer
step = 4
isConnected = 0
import serial, time, sys, os, math

# Communication série
ser=serial.Serial()
ser.baudrate=9600
ser.port="/dev/ttyACM0"

# Delai entre les respirations
respire = 3;
try:
	ser.open()
	print "\033[0;31m[PYT]\033[0m\tOUVERTURE PORT SERIE"
	isConnected = 1
except:
	print "\033[0;31m[PYT]\033[0m\tMAUVAIS PORT SERIE"
time.sleep(4) # Attendre un peu la préparation de l'arduino
print "\033[0;34m[TVS]\033[0m \033[0;32mDémarrage de la kinect\033[0m (CTRL-C pour quitter)"
try:
	import freenect
except:
	print ( "\n\033[0;34m[TVS]\033[0m \033[1;31mERREUR : Bibliothèque 'freenect' non trouvée.\033[0m" )
import subprocess

try :
	depth = freenect.sync_get_depth()[0]
except:
	print ( "\n\033[0;34m[TVS]\033[0m \033[1;31mERREUR : Impossible de récupérer la profondeur.\033[0m La kinect est-elle branchée ?" )

# Taille de l'image recue du kinect
w = 640
h = 480

# Début de la simulation de respiration

prevFrame = [[0]*w]*h

result = 0


try:
	while 1:
		time.sleep(respire) # Pour ne pas surcharger l'arduino
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
		# print "Mouvement : \t", mouvement
		if (result>99):
			print "\033[0;31m[PYT]\033[0m\tANGOISSE\n"
			envoi = 99
		elif (result<6):
			print "\033[0;31m[PYT]\033[0m\tCALME\n"
			envoi = 6
		else :
			print "\033[0;31m[PYT]\033[0m\tNORMAL\n"
			envoi = str(result).format('02d')
		respire = 180/int(envoi)
		if (isConnected):
			print ser.readline()
			ser.write(envoi)
			ser.write('\n')
		else :
			print "\033[0;31m[PYT]\033[0m\tPas connecte a l arduino\n"
			print envoi


		
except (KeyboardInterrupt, SystemExit):
	ser.write("00\n")
	freenect.Kill
	print "\n\033[0;31m[TVS]\033[0m \033[0;32mArrêt de la kinect\033[0m"
except Exception, e:
	freenect.Kill
	e = sys.exc_info()[0]
	print ( "\n\033[0;31m[TVS]\033[0m \033[1;31mERROR : %s\033[0m" % str(e) )
