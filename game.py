import pygame

from .graphics.colorspace import *
from .graphics.named_colors import *
from .graphics.geometry import *
from .graphics.font import *
from .graphics.text import *

from .physics.mathematics import *

from .cosmos import *

from .matter.graphlet.textlet import *
from .matter.graphlet.shapelet import *

from .matter.sprite.folder import *

from .trace import *

###############################################################################
class TheBigBang(Cosmos):
    def __init__(this, self: Plane, fps = 60, fgc = 0x000000, bgc = 0xFFFFFF, size = None):
        super(TheBigBang, this).__init__(fps, fgc, bgc)
        this._push_plane(self)

        if size:
            this.set_window_size(size[0], size[1])

    def construct(self, argv):
        enter_digimon_zone(argv[0])
        imgdb_setup(digimon_zonedir())

def launch_universe(world, module_name, size = None, fps = 60, trace = False):
    if trace:
        register_tracer()
    
    if module_name == "__main__":
        # 混沌初开，宇宙诞生，游戏世界就绪
        universe = TheBigBang(world(), fps, size = size)

        # 宇宙大爆炸
        # 开启游戏主循环，直到玩家关闭游戏
        universe.big_bang()

        # Python 心满意足地退出
        # 顺便销毁游戏宇宙，回归虚无
        sys.exit(0)
