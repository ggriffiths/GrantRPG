import tile
import pygame
from pygame.locals import *

defaultTile = tile.Tile(0)

class Level:
	def __init__(self,levelID=0,newName="untitled"):
		self.lvlID = levelID
		self.name = newName
		self.map = [[0 for x in xrange(50)] for x in xrange(50)]
		for i in range(50):
			for j in range(50):
				self.map[i][j]=defaultTile

      