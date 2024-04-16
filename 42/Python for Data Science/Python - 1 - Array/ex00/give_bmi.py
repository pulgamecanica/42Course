def is_float(n: int) -> bool:
    """is_float

    Details:
    Return true when the parameter is a valid float
    """
    try:
        float(n)
        return True
    except ValueError:
        return False


def give_bmi(height: list[int | float],
             weight: list[int | float]) -> list[int | float]:
    """give_bmi (Give Body Mass Index)

    Details:
    Return a list with the body mass index for the two provided lists
    The bmi is calculated by the following formula: BMI = kg / m^2

    Errors:
    When the lists contain invalid values
    If the lists are not the same size

    Reference:
    https://www.calculator.net/bmi-calculator.html
    """
    error_msg = "AssertionError: either list contains invalid values"
    assert len([_ for _ in height + weight if not is_float(_)]) == 0, error_msg
    error_msg = "AssertionError: the list are not the same size"
    assert len(weight) == len(height), error_msg
    height = [float(_) for _ in height]
    weight = [float(_) for _ in weight]
    return [_w / (_h**2) for _h, _w in zip(height, weight)]


def apply_limit(bmi: list[int | float], limit: int) -> list[bool]:
    """apply_limit

        Details:
        Apply a limit to the bmi's list
    """
    return [True if _ > limit else False for _ in bmi]
