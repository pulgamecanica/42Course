from S1E7 import Baratheon, Lannister


class King(Baratheon, Lannister):
    """
    A class representing the monster King.
    """
    def __init__(self, first_name: str, is_alive: bool = True):
        """
        Initialize a Lannister character with a first name and an initial
        state of being alive.

        Parameters:
        - first_name (str): The first name of the Stark character.
        - is_alive (bool): The initial state of the Stark character
          being alive. Default is True.
        """
        super().__init__(first_name, is_alive)

    def set_eyes(self, color: str):
        """Set king eyes."""
        self.eyes = color

    def set_hairs(self, color):
        """Set king hairs."""
        self.hairs = color

    def get_eyes(self):
        """Get the king eyes."""
        return self.eyes

    def get_hairs(self):
        """Get the king eyes."""
        return self.hairs

    _e = property(get_eyes, set_eyes)
    _h = property(get_hairs, set_hairs)
