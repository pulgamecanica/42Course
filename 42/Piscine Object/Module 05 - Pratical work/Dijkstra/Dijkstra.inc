//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __DIJKSTRA_HPP__
# define __DIJKSTRA_HPP__

#ifndef DEBUG
# define DEBUG 0
#endif

#include <iostream>
#include <utility>

# define BLACK  "\033[0;30m"
# define RED    "\033[0;31m"
# define GREEN  "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE   "\033[0;34m"
# define WHITE  "\033[0;37m"
# define ENDC   "\033[0m"

// Draw utils
void    draw_line(mlx_image_t * img, int x0, int y0, int x1, int y1, uint32_t color);
void    draw_circle(mlx_image_t * img, int xc, int yc, int r, uint32_t color);
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);

// Put Utils
//void ft_mlx_put_string(mlx_t* mlx, mlx_image_t * img, const std::string& str, int32_t x, int32_t y);
void put_img_to_img(mlx_image_t* dst, mlx_image_t* src, int x, int y);

// Geometry
bool is_inside_rect(int x, int y, int width, int height, int x0, int y0);
std::pair<int, int> ft_midpoint(int x1, int y1, int x2, int y2);

#endif
