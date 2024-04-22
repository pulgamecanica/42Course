def square(x: int or float) -> int or float:
    """Return square of a number."""
    return x ** 2


def pow(x: int or float) -> int or float:
    """Return the power of a number."""
    return x ** x


def outer(x: int or float, function) -> object:
    """
    Returns a callable function which keeps a count of the last result
    """

    count = x

    def inner() -> float:
        """Function callback for the callable object returned"""
        nonlocal count
        count = function(count)
        return count

    return inner
