from abc import *       # abstract base class

from .image import game_save_image

###############################################################
class IDisplay(ABC):
    def __init__(self):
        self.__update_sequence_depth = 0
        self.__update_is_needed = False

    @abstractmethod
    def refresh(self): pass

    @abstractmethod
    def log_message(self, fgc, message): pass

    @abstractmethod
    def start_input_text(self, prompt): pass

    @abstractmethod
    def snapshot(self): pass

    def save_snapshot(self, pname):
        game_save_image(self.snapshot(), pname)

    def begin_update_sequence(self):
        self.__update_sequence_depth += 1

    def in_update_sequence(self):
        return self.__update_sequence_depth > 0

    def end_update_sequence(self):
        self.__update_sequence_depth -= 1

        if self.__update_sequence_depth < 1:
            self.__update_sequence_depth = 0

            if self.update_is_needed:
                self.refresh()
                self.__update_is_needed = False

    def needs_update(self):
        return self.__update_is_needed

    def notify_updated(self):
        if self.in_update_sequence():
            self.__update_is_needed = True
        else:
            self.refresh()
            self.__update_is_needed = False

