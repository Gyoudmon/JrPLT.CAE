#!/usr/bin/env python3

from ...big_bang.game import *

###############################################################################
class GameOfLifelet(IGraphlet):
    def __init__(self, size, gridsize = 8.0):
        # 通过父类的构造函数设置窗口标题和帧频
        super(GameOfLifelet, self).__init__()

        if isinstance(size, int):
            self.row, self.col = size, size
        else:
            self.row, self.col = size

        self.color = BLACK
        self.generation = 0
        self.gridsize = gridsize
        self.hide_grid = False
        self.world = None
        self.shadow = None

    def construct(self):
        super(GameOfLifelet, self).construct()
        self.shadow = [0] * (self.row * self.col)
        self.world = [[0 for c in range(self.col)]
                       for r in range(self.row)]

    def get_extent(self, x, y):
        w = math.ceil(self.gridsize * self.col) + 1.0
        h = math.ceil(self.gridsize * self.row) + 1.0

        return w, h

    def draw(self, renderer, x, y, Width, Height):
        game_draw_rect(renderer, x, y, Width - 1, Height - 1, self.color)

        if not self.hide_grid:
            game_draw_grid(renderer, self.row, self.col, self.gridsize, self.gridsize, x, y, self.color)
        
        game_fill_grid(renderer, self.world, self.row, self.col, self.gridsize, self.gridsize, x, y, self.color)

###############################################################################
    def construct_random_world(self):
        for r in range(self.row):
            for c in range(self.col):
                if random.randint(1, 100) % 2 == 0:
                    self.world[r][c] = 1
                else:
                    self.world[r][c] = 0

        self.generation = 0

    def pace_forward(self, repeats):
        pass

###############################################################################
    def show_grid(self, yes):
        if self.hide_grid == yes:
            self.hide_grid = not yes
            self.notify_updated()

    def set_color(self, hex):
        if self.color != hex:
            self.color = hex
            self.notify_updated()
