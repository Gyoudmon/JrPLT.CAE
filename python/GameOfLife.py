#!/usr/bin/env python3

from digitama.big_bang.game import *        # 导入游戏模块，内含 Plane 类和常用函数
from digitama.basis.conway.lifelet import * # 导入游戏图元

import enum

###############################################################################
default_frame_rate = 8
generation_fmt = "Generation: %d"

AUTO_KEY = 'a'
STOP_KEY = 's'
PACE_KEY = 'p'
EDIT_KEY = 'e'
LOAD_KEY = 'l'
RAND_KEY = 'r'
RSET_KEY = 'z'
WRTE_KEY = 'w'

ordered_keys = (AUTO_KEY, STOP_KEY, PACE_KEY, EDIT_KEY, LOAD_KEY, WRTE_KEY, RAND_KEY, RSET_KEY)
colors_for_auto = (GRAY, GREEN, GRAY, GRAY, GRAY, GRAY, GRAY, GRAY)
colors_for_stop = (GREEN, GRAY, GREEN, GREEN, GRAY, GRAY, GRAY, GRAY)
colors_for_edit = (GREEN, GRAY, GREEN, GRAY, GREEN, GREEN, GREEN, GREEN)

###############################################################################
class GameState(enum.Enum):
    Auto = 0,
    Stop = 1,
    Edit = 2,
    _ = 3

###############################################################################
class GameOfLifeWorld(Plane):
    def __init__(self, gridsize = 8.0):
        # 通过父类的构造函数设置窗口标题和帧频
        super(GameOfLifeWorld, self).__init__("生命游戏")

        # 本游戏世界有以下物体
        self.gameboard : GameOfLifelet = None
        self.generation : Labellet = None
        self.instructions  = {}

        # 私有变量
        self.__gridsize = gridsize

    def load(self, width, height):
        border_height = height - 108.0
        border_width = border_height
        col = round(border_width / self.__gridsize) - 1
        row = round(border_height / self.__gridsize) - 1

        self.gameboard = self.insert(GameOfLifelet((row, col), self.__gridsize))
        self.generation = self.insert(Labellet(generation_fmt % (self.gameboard.generation), GameFont.math, GREEN))

        self.instructions[AUTO_KEY] = self.insert(Labellet("%c. 自行演化" % AUTO_KEY, GameFont.monospace))
        self.instructions[STOP_KEY] = self.insert(Labellet("%c. 停止演化" % STOP_KEY, GameFont.monospace))
        self.instructions[EDIT_KEY] = self.insert(Labellet("%c. 手动编辑" % EDIT_KEY, GameFont.monospace))
        self.instructions[RAND_KEY] = self.insert(Labellet("%c. 随机重建" % RAND_KEY, GameFont.monospace))
        self.instructions[RSET_KEY] = self.insert(Labellet("%c. 世界归零" % RSET_KEY, GameFont.monospace))
        self.instructions[PACE_KEY] = self.insert(Labellet("%c. 单步跟踪" % PACE_KEY, GameFont.monospace))
        self.instructions[LOAD_KEY] = self.insert(Labellet("%c. 载入范例" % LOAD_KEY, GameFont.monospace))
        self.instructions[WRTE_KEY] = self.insert(Labellet("%c. 保存范例" % WRTE_KEY, GameFont.monospace))

    def reflow(self, width, height):
        self.move_to(self.gameboard, (width * 0.5, height * 0.5), MatterAnchor.CC)
        self.move_to(self.generation, (self.gameboard, MatterAnchor.RT), MatterAnchor.RB)

        self.move_to(self.instructions[ordered_keys[0]], (0.0, height), MatterAnchor.LB)
        for idx in range(1, len(ordered_keys)):
            self.move_to(self.instructions[ordered_keys[idx]],
                    (self.instructions[ordered_keys[idx - 1]], MatterAnchor.RB),
                    MatterAnchor.LB, 16.0)

    def update(self, count, interval, uptime):
        self.gameboard.pace_forward(1)
    
# protected
    def on_mission_start(self, width, height):
        self.__update_instructions_state(colors_for_auto)

# private
    def __update_instructions_state(self, colors):
        for idx in range(len(ordered_keys)):
            self.instructions[ordered_keys[idx]].set_text_color(colors[idx])

###############################################################################
launch_universe(GameOfLifeWorld, __name__)
