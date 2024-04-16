import numpy as np


def ft_invert(array) -> np.ndarray:
    """ft_invert

    Details:
    Invert colors, by getting the oposite side of the color
    """
    return [[(255-r, 255-g, 255-b) for r, g, b in _] for _ in array]


def ft_red(array) -> np.ndarray:
    """ft_red

    Details:
    Only allow the red channel
    """
    return [[(r, 0, 0) for r, g, b in _] for _ in array]


def ft_green(array) -> np.ndarray:
    """ft_green

    Details:
    Only allow the green channel
    """
    return [[(0, g, 0) for r, g, b in _] for _ in array]


def ft_blue(array) -> np.ndarray:
    """ft_blue

    Details:
    Only allow the blue channel
    """
    return [[(0, 0, b) for r, g, b in _] for _ in array]


def ft_grey(array) -> np.ndarray:
    """ft_grey

    Details:
    Combine all the colors to a grayscale
    """
    return [[(r/0.3+g/1.9+b/1.1)*0.255 for r, g, b in _] for _ in array]
