from abc import ABC, abstractmethod


class Character(ABC):
    """
    An abstract class representing a character.
    """

    @abstractmethod
    def __init__(self, first_name: str, is_alive: bool = True):
        """
        Initialize the character with a first name and an initial
        state of being alive.

        Parameters:
        - first_name (str): The first name of the character.
        - is_alive (bool): The initial state of the character being alive.
          Default is True.
        """
        self.first_name = first_name
        self.is_alive = is_alive

    def die(self) -> None:
        """
        Change the state of the character being alive to false.
        """
        self.is_alive = False


class Stark(Character):
    """
    A class representing a member of House Stark.
    Inherits from the Character class.
    """

    def __init__(self, first_name: str, is_alive: bool = True):
        """
        Initialize a Stark character with a first name and an initial
        state of being alive.

        Parameters:
        - first_name (str): The first name of the Stark character.
        - is_alive (bool): The initial state of the Stark character
          being alive. Default is True.
        """
        super().__init__(first_name, is_alive)
