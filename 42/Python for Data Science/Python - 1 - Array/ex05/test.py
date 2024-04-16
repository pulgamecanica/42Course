from pimp_image import ft_invert, ft_grey, ft_red, ft_green, ft_blue
from load_image import ft_load
from matplotlib import pyplot as plt
from matplotlib.widgets import Button
# https://matplotlib.org/3.8.3/api/_as_gen/matplotlib.axes.Axes.imshow.html#matplotlib.axes.Axes.imshow


class ImageSwitcher:
    """ImageSwitcher

        Details:
        Gadget to switch images on a plot
        """
    def __init__(self, images):
        self.images = images
        self.current_image = 0

        self.fig, self.ax = plt.subplots()
        plt.subplots_adjust(bottom=0.2)
        self.img = self.ax.imshow(self.images[self.current_image], cmap='gray')

        self.ax_prev = plt.axes([0.7, 0.05, 0.1, 0.075])
        self.ax_next = plt.axes([0.81, 0.05, 0.1, 0.075])
        self.btn_prev = Button(self.ax_prev, 'Previous')
        self.btn_next = Button(self.ax_next, 'Next')

        self.btn_prev.on_clicked(self.prev_image)
        self.btn_next.on_clicked(self.next_image)

    def prev_image(self, event):
        """prev_image

        Details:
        Continue to the last image
        """
        self.current_image = (self.current_image - 1) % len(self.images)
        self.img.set_data(self.images[self.current_image])
        self.fig.canvas.draw()

    def next_image(self, event):
        """next_image

        Details:
        Continue to the next image
        """
        self.current_image = (self.current_image + 1) % len(self.images)
        self.img.set_data(self.images[self.current_image])
        self.fig.canvas.draw()


def main():
    im = ft_load("landscape.jpg")
    images = [im, ft_grey(im), ft_blue(im),
              ft_green(im), ft_red(im), ft_invert(im)]
    ImageSwitcher(images)
    plt.show()


if __name__ == '__main__':
    main()
