#include "utils.h"
#include <unistd.h>
#include <mlx.h>

t_img	new_img(int w, int h, t_win window) {
	t_img	image;

	image.win = window;
	image.img_ptr = mlx_new_image(window.mlx_ptr, w, h);
	image.addr = mlx_get_data_addr(image.img_ptr, &(image.bpp),
			&(image.line_len), &(image.endian));
	image.w = w;
	image.h = h;
	return (image);
}

t_img	new_file_img(char * path, t_win window) {
	t_img image;

	image.win = window;
	image.img_ptr = mlx_xpm_file_to_image(window.mlx_ptr, path, &image.w, &image.h);
	if (!image.img_ptr)
		write(2, "File could not be read\n", 23);
	else
		image.addr = mlx_get_data_addr(image.img_ptr, &(image.bpp),
			&(image.line_len), &(image.endian));
	return (image);
}

void	destroy_image(t_img img) {
	if (img.img_ptr && img.win.mlx_ptr) 
       mlx_destroy_image(img.win.mlx_ptr, img.img_ptr);
}

void	put_pixel_img(t_img img, int x, int y, int color) {
	char	*dst;

	if (x >= 0 && y >= 0 && x < img.w && y < img.h) {
		dst = img.addr + (y * img.line_len + x * (img.bpp / 8));
		*(unsigned int *) dst = color;
	}
}

unsigned int	get_pixel_img(t_img img, int x, int y) {
	return (*(unsigned int *)((img.addr
			+ (y * img.line_len) + (x * img.bpp / 8))));
}

static int	put_pixel_valid(t_img img, int x, int y) {
	if (x >= 0 && y >= 0 && x < img.w && y < img.h) {
		return (*(unsigned int *)(img.addr + (y * img.line_len + x * (img.bpp / 8))) != 0xFF000000 &&
		*(unsigned int *)(img.addr + (y * img.line_len + x * (img.bpp / 8))) != 0x00000000);
	}
	return 0;
}

void	put_img_to_img(t_img dst, t_img src, int x, int y) {
	int i;
	int j;

	i = 0;
	while(i < src.w) {
		j = 0;
		while (j < src.h) {
			if (put_pixel_valid(src, i, j))
				put_pixel_img(dst, x + i, y + j, get_pixel_img(src, i, j));
			j++;
		}
		i++;
	}
}