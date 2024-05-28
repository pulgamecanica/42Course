extern "C" {
  #include <stdlib.h>
}

#include "MLX42/MLX42.h"

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
return (r << 24 | g << 16 | b << 8 | a);
}

static void ft_mlx_put_pixel(mlx_image_t * img, int x, int y, uint32_t color) {
  if (x < 0 || y < 0 || y >= (int)img->height || x >= (int)img->width)
    return;
  mlx_put_pixel(img, x, y, color);
}

void draw_line(mlx_image_t * img, int x0, int y0, int x1, int y1, uint32_t color) {
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;) {
      ft_mlx_put_pixel(img, x0, y0, color);
    if (x0==x1 && y0==y1)
      break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
  }
}  

static void draw_circle_coords(mlx_image_t * img, int xc, int yc, int x, int y, uint32_t color) {
  // ft_mlx_put_pixel(img, xc+x, yc+y, color);
  // ft_mlx_put_pixel(img, xc-x, yc+y, color);
  draw_line(img, xc+x, yc+y, xc-x, yc+y, color);

  // ft_mlx_put_pixel(img, xc+x, yc-y, color);
  // ft_mlx_put_pixel(img, xc-x, yc-y, color);
  draw_line(img, xc+x, yc-y, xc-x, yc-y, color);
  
  // ft_mlx_put_pixel(img, xc+y, yc+x, color);
  // ft_mlx_put_pixel(img, xc-y, yc+x, color);
  draw_line(img, xc+y, yc+x, xc-y, yc+x, color);
  
  // ft_mlx_put_pixel(img, xc+y, yc-x, color);
  // ft_mlx_put_pixel(img, xc-y, yc-x, color);
  draw_line(img, xc+y, yc-x, xc-y, yc-x, color);
}

void draw_circle(mlx_image_t * img, int xc, int yc, int r, uint32_t color) {
  int x = 0, y = r;
  int d = 3 - 2 * r;
  while (y >= x) {
    /*for each pixel we will draw all eight pixels */
    draw_circle_coords(img, xc, yc, x, y, color);
    x++;
    /*check for decision parameter and correspondingly update d, x, y*/
    if (d > 0) {
      y--;
      d = d + 4 * (x - y) + 10;
    } else {
      d = d + 4 * x + 6;
    }
    draw_circle_coords(img, xc, yc, x, y, color);
  }
}

