//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __MENU_TUTORIAL_H__
# define __MENU_TUTORIAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "MLX42.h"
#include "libft.h"


# define WIDTH 512
# define HEIGHT 512


// # define BLACK	"\033[0;30m"
// # define RED	"\033[0;31m"
// # define GREEN	"\033[0;32m"
// # define YELLOW	"\033[0;33m"
// # define BLUE	"\033[0;34m"
// # define WHITE	"\033[0;37m"
// # define ENDC	"\033[0m"

/* Draw Functions */
void draw_circle(mlx_image_t * img, int xc, int yc, int r, uint32_t color);
void bezier_curve(mlx_image_t * img, int x[], int y[], uint32_t color);
void draw_line(mlx_image_t * img, int x0, int y0, int x1, int y1, uint32_t color);

int32_t mlx_get_pixel(mlx_image_t* image, uint32_t x, uint32_t y);
void    put_img_to_img_scale(mlx_image_t * dst, mlx_image_t * src, int x, int y, double scale);

#endif
