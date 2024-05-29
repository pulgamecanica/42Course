extern "C" {
  #include <stdlib.h>
}

#include <string>
#include <cstring>
#include <utility>

#include "MLX42/MLX42.h"


# define FONT_WIDTH 10
# define FONT_HEIGHT 20

#define BPP sizeof(int32_t)

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a) {
  return (r << 24 | g << 16 | b << 8 | a);
}

std::pair<int, int> ft_midpoint(int x1, int y1, int x2, int y2) { 
  return std::pair<int, int>({(float)(x1 + x2) / 2.0, (float)(y1 + y2) / 2.0});
} 

// x and y are the middle of the square, not the corner
bool is_inside_rect(int x, int y, int width, int height, int x0, int y0) {
  int left_edge_x = x - (width / 2.0);
  int upper_edge_y = y - (height / 2.0);
  return x0 > left_edge_x && x0 < left_edge_x + width && y0 > upper_edge_y && y0 < upper_edge_y + height;
}

static void ft_mlx_put_pixel(mlx_image_t * img, int x, int y, uint32_t color) {
  if (x < 0 || y < 0 || y >= (int)img->height || x >= (int)img->width)
    return;
  mlx_put_pixel(img, x, y, color);
}

static int32_t mlx_get_pixel(mlx_image_t* image, uint32_t x, uint32_t y) {
  if (x > image->width || y > image->height)
    return 0xFF000000;
  uint8_t* pixelstart = image->pixels + (y * image->width + x) * BPP;
  return ft_pixel(*(pixelstart), *(pixelstart + 1),
    * (pixelstart + 2), *(pixelstart + 3));
}

void  put_img_to_img(mlx_image_t* dst, mlx_image_t* src, int x, int y) {
  unsigned i;
  unsigned j;

  i = 0;
  while(i < src->width) {
    j = 0;
    while (j < src->height) {
      ft_mlx_put_pixel(dst, x + i, y + j, mlx_get_pixel(src, i, j));
      j++;
    }
    i++;
  }
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

