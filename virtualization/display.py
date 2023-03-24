from abc import *               # abstract base class

from ..graphics.image import *

###############################################################
class IDisplay(ABC):
    def __init__(self):
        self.__update_sequence_depth = 0
        self.__update_is_needed = False

    @abstractmethod
    def get_extent(self): pass

    def get_client_extent(self):
        return self.get_extent()

    @abstractmethod
    def snapshot(self): pass

    def save_snapshot(self, pname):
        snapshot_png = self.snapshot()
        okay = game_save_image(snapshot_png, pname)

        game_unload_image(snapshot_png)

        return okay

    @abstractmethod
    def refresh(self): pass

    def begin_update_sequence(self):
        self.__update_sequence_depth += 1

    def is_in_update_sequence(self):
        return self.__update_sequence_depth > 0

    def end_update_sequence(self):
        self.__update_sequence_depth -= 1

        if self.__update_sequence_depth < 1:
            self.__update_sequence_depth = 0

            if self.should_update():
                self.refresh()
                self.__update_is_needed = False

    def should_update(self):
        return self.__update_is_needed

    def notify_updated(self):
        if self.is_in_update_sequence():
            self.__update_is_needed = True
        else:
            self.refresh()
            self.__update_is_needed = False
