from S1E9 import Character


class Baratheon(Character):
    """
    A class representing a member of House Baratheon.
    """

    def __init__(self, first_name: str, is_alive: bool = True):
        """
        Initialize a Baratheon character with a first name and an
        initial state of being alive.

        Parameters:
        - first_name (str): The first name of the Stark character.
        - is_alive (bool): The initial state of the Stark character
          being alive. Default is True.
        """
        self.first_name = first_name
        self.is_alive = is_alive
        self.family_name = "Baratheon"
        self.eyes = "brown"
        self.hairs = "dark"

    def __str__(self) -> str:
        return f"Vector: ('{self.family_name}', '{self.eyes}', '{self.hairs}')"

    def __repr__(self) -> str:
        return f"Vector: ('{self.family_name}', '{self.eyes}', '{self.hairs}')"

    def create_baratheon(name: str, is_alive: bool):
        return Baratheon(name, is_alive)


class Lannister(Character):
    """
    A class representing a member of House Lannister.
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
        self.first_name = first_name
        self.is_alive = is_alive
        self.family_name = "Lannister"
        self.eyes = "blue"
        self.hairs = "light"

    def __str__(self) -> str:
        return f"Vector: ('{self.family_name}', '{self.eyes}', '{self.hairs}')"

    def __repr__(self) -> str:
        return f"Vector: ('{self.family_name}', '{self.eyes}', '{self.hairs}')"

    def create_lannister(name: str, is_alive: bool):
        return Lannister(name, is_alive)
