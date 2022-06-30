/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SnakeGame.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname- <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 4242/42/42 42:42:42 by somename-         #+#    #+#             */
/*   Updated: 4242/42/42 42:42:42 by somename-        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "canvas.h"

/* 
new_program - Generates a new program & window
  Parameters:
    [w]   Width of the new program
    [h]   Height of the new program
    [str] Title displayed on top  of the program
  Return:
    An instance of the t_win structure
    which also contains the instance to the mlx_ptr
*/
t_win	new_program(int w, int h, char *str)
{
	void	*mlx_ptr;

	mlx_ptr = mlx_init();
	return ((t_win){mlx_ptr, mlx_new_window(mlx_ptr, w, h, str), w, h});
}

/* Display information relevant about an image */
static void	img_info(t_img image)
{
	printf("********* Configurations *********\n");
	printf("img_ptr		: [%p]\n", image.img_ptr);
	printf("bpp		: [%d]\n", image.bpp);
	printf("line_len	: [%d]\n", image.line_len);
	printf("endian		: [%d]\n", image.endian);
	printf("addr		: [%p]\n", image.addr);
	printf("-----------------------------------\n");
}

/*
new_img - Generates a new image 
  Parameters:
    [w]      Width of the image generated
    [h]      Height of the image generated
    [window] Window to whom the image will belong
  Return:
    An instance of the t_img structure
    it also containes an instance to the window
*/
t_img	new_img(int w, int h, t_win window)
{
	t_img	image;

	image.win = window;
	image.img_ptr = mlx_new_image(window.mlx_ptr, w, h);
	image.addr = mlx_get_data_addr(image.img_ptr, &(image.bpp),
			&(image.line_len), &(image.endian));
	image.w = w;
	image.h = h;
	img_info(image);
	return (image);
}

/*
put_pixel_img - Place one pixel on the given image
  Parameters
    [img]   Image where the pixel will be placed
    [x]     Pixel column
    [y]     Pixel row
    [color] Pixel color
*/
void	put_pixel_img(t_img img, int x, int y, int color)
{
	char	*dst;

	dst = img.addr + (y * img.line_len + x * (img.bpp / 8));
	*(unsigned int *) dst = color;
}

int	get_pixel_img(t_img img, int x, int y)
{
	char	*dst;

	if (x >= img.w || y >= img.h || x < 0 || y < 0)
		return (0xFF);
	dst = img.addr + (y * img.line_len + x * (img.bpp / 8));
	return (*(unsigned int *) dst);
}
