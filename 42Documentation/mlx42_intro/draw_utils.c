#include <stdlib.h>
#include "MLX42/MLX42.h"

void draw_line(mlx_image_t * img, int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;) {
        mlx_put_pixel(img, x0, y0, color);
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
    // mlx_put_pixel(img, xc+x, yc+y, color);
    // mlx_put_pixel(img, xc-x, yc+y, color);
    draw_line(img, xc+x, yc+y, xc-x, yc+y, color);

    // mlx_put_pixel(img, xc+x, yc-y, color);
    // mlx_put_pixel(img, xc-x, yc-y, color);
    draw_line(img, xc+x, yc-y, xc-x, yc-y, color);
    
    // mlx_put_pixel(img, xc+y, yc+x, color);
    // mlx_put_pixel(img, xc-y, yc+x, color);
    draw_line(img, xc+y, yc+x, xc-y, yc+x, color);
    
    // mlx_put_pixel(img, xc+y, yc-x, color);
    // mlx_put_pixel(img, xc-y, yc-x, color);
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

// // x & y must be a 4 dimention array
// void bezier_curve(mlx_image_t * img, int x[], int y[], uint32_t color)
// {
//     double xu = 0.0 , yu = 0.0 , u = 0.0 ;
//     for(u = 0.0 ; u <= 1.0 ; u += 0.0001)
//     {
//         xu = pow(1-u,3)*x[0]+3*u*pow(1-u,2)*x[1]+3*pow(u,2)*(1-u)*x[2]
//              +pow(u,3)*x[3];
//         yu = pow(1-u,3)*y[0]+3*u*pow(1-u,2)*y[1]+3*pow(u,2)*(1-u)*y[2]
//             +pow(u,3)*y[3];
//         if ((uint32_t)xu > 0 && (uint32_t)xu < img->width && (uint32_t)yu > 0 && (uint32_t)yu < img->height)
//             mlx_put_pixel(img, (int)xu, (int)yu, color) ;
//     }
// }

