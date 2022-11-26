from abc import *       # Abstract Base Class

from .display import IDisplay

###############################################################
class IScreen(ABC):
    @abstractmethod
    def display(self) -> IDisplay: pass
        
    @abstractmethod
    def refresh(self): pass

    @abstractmethod
    def begin_update_sequence(self): pass
    
    @abstractmethod
    def in_update_sequence(self): pass

    @abstractmethod
    def end_update_sequence(self): pass
    
    @abstractmethod
    def needs_update(self): pass

    @abstractmethod
    def notify_updated(self): pass

    @abstractmethod
    def log_message(self, fgc, message): pass

    @abstractmethod
    def start_input_text(self, prompt): pass

