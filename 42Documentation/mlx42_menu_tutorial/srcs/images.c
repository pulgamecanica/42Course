#include "menu_tutorial.h"

#define BPP sizeof(int32_t)

static int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

int32_t mlx_get_pixel(mlx_image_t* image, uint32_t x, uint32_t y) {
    if (x > image->width || y > image->height)
        return 0;

    uint8_t * pixelstart = image->pixels + (y * image->width + x) * BPP;
    return get_rgba(*(pixelstart), *(pixelstart + 1),
            *(pixelstart + 2), *(pixelstart + 3));
}

static int  put_pixel_valid(mlx_image_t * img, uint32_t x, uint32_t y) {
    return (x < img->width && y < img->height);
}

/**
 * scaled_W -> originalW
 * scaledX -. ?
 * 
 * Formula: x = (scaledX * originalW) / scaled_W
 * Formula: y = (scaledY * originalH) / scaled_H 
 */ 
void    put_img_to_img_scale(mlx_image_t * dst, mlx_image_t * src, int x, int y, double scale) {
    int scaled_w;
    int scaled_h;
    int i;
    int j;

    if (scale < 0)
        return ;
    scaled_w = src->width * scale;
    scaled_h = src->height * scale;

    i = 0;
    while(i < scaled_w) {
        j = 0;
        while (j < scaled_h) {
            int _i;
            int _j;

            _i =  (i * src->width) / scaled_w;
            _j = (j * src->height) / scaled_h;
            if (put_pixel_valid(src, _i, _j)) {
                uint32_t color;
                color = mlx_get_pixel(src, _i, _j);
                if (color && color != 0xFFFFFF00)
                    mlx_put_pixel(dst, x + i, y + j, color);
            }
            j++;
        }
        i++;
    }
}