from typing import Any

# https://en.wikipedia.org/wiki/Standard_deviation

"""
Variance: How much the value varies from the mean

Standard deviation: The square root of the variance.
It provides a measure of the average distance of each data point from the mean.
Standard deviation is useful because it is expressed in the same units as the
original data.
"""


def ft_statistics(*args: Any, **kwargs: Any) -> None:
    """
    Calculate statistics based on provided arguments and keyword arguments.

    Args:
        *args: Any number of arguments. Can be numbers or lists of numbers.
        **kwargs: Keyword arguments specifying which statistics to calculate.
            Each keyword argument should be named after the statistic to
            calculate and set to True to include it in the calculation.

    Raises:
        TypeError: If arguments are not numbers.

    Returns:
        None: Prints the calculated statistics.
    """
    numbers = []
    for arg in args:
        if isinstance(arg, (int, float)):
            numbers.append(arg)
        else:
            raise TypeError("Arguments must be numbers.")

    stats_functions = {
        "mean":
            lambda x: sum(x) / len(x) if x else None,
        "median":
            lambda x: sorted(x)[len(x) // 2] if len(x) % 2 != 0
            else (sorted(x)[len(x) // 2 - 1] + sorted(x)[len(x) // 2]) / 2
            if x else None,
        "quartile": lambda x: [
            float(sorted(x)[int(len(x) * 0.25)]),
            float(sorted(x)[int(len(x) * 0.75)])] if x else None,
        "std": lambda x: (
            sum([(i - sum(x) / len(x)) ** 2 for i in x]) / len(x)) ** 0.5
            if x else None,
        "var": lambda x: sum([(i - sum(x) / len(x)) ** 2 for i in x]) / len(x)
            if x else None
    }

    for stat in kwargs.values():
        if stat in stats_functions:
            res = stats_functions[stat](numbers)
            if res:
                print(f"{stat} : {res}")
            else:
                print("ERROR")
