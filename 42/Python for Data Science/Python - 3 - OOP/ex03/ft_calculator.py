class calculator:
    """
    Class which is representing a calculator
    """
    def __init__(self, vector: list[float]):
        self.vector = vector

    def __add__(self, object) -> None:
        """
        Add to each member
        """
        self.vector = self.vector = [_ + object for _ in self.vector]
        print(self.vector)

    def __mul__(self, object) -> None:
        """
        Multiply for each member
        """
        self.vector = self.vector = [_ * object for _ in self.vector]
        print(self.vector)

    def __sub__(self, object) -> None:
        """
        Substract to each member
        """
        self.vector = self.vector = [_ - object for _ in self.vector]
        print(self.vector)

    def __truediv__(self, object) -> None:
        """
        Divide for each member

        Errors:
        AssertionError when the divisor is zero
        """
        assert object, "Cannot divide by zero."
        self.vector = self.vector = [_ // object for _ in self.vector]
        print(self.vector)
