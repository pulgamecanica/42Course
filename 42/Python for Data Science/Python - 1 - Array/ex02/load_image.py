from PIL import Image
import numpy as np

# https://pillow.readthedocs.io/en/stable/reference/Image.html#open-rotate-and-display-an-image-using-the-default-viewer


def ft_load(path: str) -> np.ndarray:
    """ft_load

    Details:
    Read an image pointed by path
    Print the shape of it and return an array
    representing each pixel [R G B]"""
    try:
        im = np.array(Image.open(path))
        print(f"The shape of the image is: {im.shape}")
        return im
    except (FileNotFoundError, AttributeError) as msg:
        print(msg)
