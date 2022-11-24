from .game import *

###############################################################################
class World(Universe):
    def __init__(self, title, fps = 60, fgc = 0x000000, bgc = 0xFFFFFF):
        super(World, self).__init__(fps, fgc, bgc)