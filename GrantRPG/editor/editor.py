import os
import Tkinter
import level
import tile
import pygame
from pygame.locals import *

# Enum for level editor states
class ActionMode:
	swap, place, delete = range(3)

# Enum for level editor states
class TileMode:
	tile1, tile2, tile3 = range(3)

# Initialize
currentLevel = level.Level()
baseDir = "/levels/"
ext = ".txt"
currentDir = baseDir + currentLevel.name + ext
editorMode = ActionMode.swap
selectedTile = TileMode.tile1 

# Send map array to .txt
def editorImport(fileName):
	with open(fileName, 'r') as textFile:									
		fileData = [line.strip() for line in lvlFile]
	return fileData

# Get map array from .txt
def editorExport(fileName,data):
	lvlFile.write(fileName)
	for line in data:
		lvlFile.write("%s\n" % line)
	
def mouseClick(x,y,button):
	# calculate which tile to alter
	tX = 0
	tY = 0

	if mode==swap:
		if button == "left":
			currentLevel.map[tX][tY].tileInc()
		else:
			currentLevel.map[tX][tY].tileDec()

	elif mode == place:
		currentLevel.map[tX][tY].tilePlace(selectedTile)

	elif mode == delete:
		currentLevel.map[tX][tY].tileDelete()

background_color = (0,0,0)
(width, height) = (800,600)

screen = pygame.display.set_mode((width, height))
pygame.display.set_caption('Level Editor')
screen.fill(background_color)


def lvlDraw():
	for i in range(25):
		for j in range(25):
			pygame.draw.rect(screen,(255,0,0), (i*20,j*20,15,15), 0)
 



running = True
while running:
	lvlDraw()
	pygame.display.flip()
 	for event in pygame.event.get():
 		if event.type == pygame.QUIT:
 			running = False
