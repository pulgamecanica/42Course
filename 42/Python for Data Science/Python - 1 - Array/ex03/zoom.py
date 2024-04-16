import numpy as np
from matplotlib import pyplot as plt
from load_image import ft_load
# https://matplotlib.org/3.8.3/api/_as_gen/matplotlib.axes.Axes.imshow.html#matplotlib.axes.Axes.imshow


def main():
    im = ft_load("animal.jpeg")
    # Slice it
    start_horz = 450
    end_horz = 850
    start_vert = 100
    end_vert = 500
    if start_horz >= end_horz or start_vert >= end_vert:
        print("WoW, careful there, not a valid slice")
        return
    sliced_im = im[start_vert:end_vert, start_horz:end_horz]
    # Channel Gray
    grayscale_table = np.array([[0.3], [0.19], [0.11]])
    gray_array = np.dot(sliced_im, grayscale_table)
    # Print info
    print(f"Grayscale: {[[0.3], [0.19], [0.11]]}")
    print(f"New shape after slicing: {sliced_im.shape}")
    print(sliced_im)
    print("New img after grayscale:")
    print(gray_array)
    # Show plot
    plt.imshow(gray_array, cmap="gray")
    plt.show()


if __name__ == '__main__':
    main()
