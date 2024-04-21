class calculator:
    """
    Class which is representing a calculator
    """

    @staticmethod
    def dotproduct(V1: list[float], V2: list[float]) -> None:
        "Sum of the multiplication of all elements"
        print("Dot product is:", sum([a * b for a, b in zip(V1, V2)]))

    @staticmethod
    def add_vec(V1: list[float], V2: list[float]) -> None:
        "Sum each pair of elements"
        print("Add Vector is :", [float(a + b) for a, b in zip(V1, V2)])

    @staticmethod
    def sous_vec(V1: list[float], V2: list[float]) -> None:
        "Substract each pair of elements"
        print("Sous Vector is:", [float(a - b) for a, b in zip(V1, V2)])
