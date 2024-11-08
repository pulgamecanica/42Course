//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <mlx.h>
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define GAME_NAME "Ft_Game"
#define WIDTH 420
#define HEIGHT 300
#define PADDING 42
#define TRACKER_Y HEIGHT - 50
#define TRACKER_H 10

static t_data new_game_data(t_win *win) {
    t_img *bg;
    t_img *fg;
    t_img *im1;
    t_img *im2;

    bg = new_img(WIDTH, HEIGHT, win);
    fg = new_img(WIDTH, HEIGHT, win);
    if (!bg || !fg)
        exit(EXIT_FAILURE);
    for (int i = 0; i < bg->w; i++) {
        for (int j = 0; j < bg->h; j++) {
            put_pixel_img(bg, i, j, 0xFFE5B4);
            if (j >= TRACKER_Y && j <= TRACKER_Y + TRACKER_H && i > PADDING && i < WIDTH - PADDING)
                put_pixel_img(bg, i, j, 0xB5CFFF);
        }
    }
    im1 = new_file_img("GreyWall.xpm", win);
    im2 = new_file_img("BlueWall.xpm", win);
    if (!im1 || !im2)
        exit(EXIT_FAILURE);
    if (im1->w != im2->w || im1->h != im2->h) {
        printf("ERROR, wrong size [%d, %d] vs [%d, %d]!\n", im1->w, im1->h, im2->w, im2->h);
        exit(EXIT_FAILURE);
    }
    printf("Done Creating Game Data\n");
    return (t_data){win, bg, fg, im1, im2, 0, 2000};
}

// Linear interpolation function for color
static int color_lerp(int color1, int color2, float t) {
    if (color1 == 0xFF000000)
        return color2;
    if (color2 == 0xFF000000)
        return color1;
    int r1 = get_r(color1);
    int g1 = get_g(color1);
    int b1 = get_b(color1);

    int r2 = get_r(color2);
    int g2 = get_g(color2);
    int b2 = get_b(color2);

    // Interpolate each component
    int r = r1 + t * (r2 - r1);
    int g = g1 + t * (g2 - g1);
    int b = b1 + t * (b2 - b1);

    // Combine interpolated components into a single color (0xAARRGGBB)
    // Assuming we keep full opacity (0xFF000000)
    int interpolated_color = (0xFF << 24) | (r << 16) | (g << 8) | b;

    return interpolated_color;
}

static t_img *get_interpolation(t_data * data) {
    t_img *img;
    int width;
    int height;

    width = data->im1->w;
    height = data->im1->h;
    img = new_img(data->im1->w, data->im1->h, data->win);
    if (!img)
        exit(EXIT_FAILURE);
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            put_pixel_img(img, x, y,
                color_lerp(
                    get_pixel_img(data->im1, x, y),
                    get_pixel_img(data->im2, x, y),
                    (float)data->current_iteration / (float)data->total_iterations
                )
            );

    return img;
}

static void draw_foreground(t_data *data) {
    float t;
    int x;
    int start;
    int end;
    char num_str[42];

    put_img_to_img(data->fg, data->bg, 0, 0);
    start = PADDING;
    end = WIDTH - PADDING;
    t = (float)data->current_iteration / (float)data->total_iterations;
    x = (start + t * (end - start));
    for (int y = TRACKER_Y; y < TRACKER_Y + TRACKER_H; y++)
        put_pixel_img(data->fg, x, y, 0xFF0000);
    snprintf(num_str, sizeof(num_str), "%.2f%%", t * 100);
    mlx_put_image_to_window(
        data->win->mlx_ptr,
        data->win->win_ptr,
        data->fg->img_ptr, 0, 0);
    // mlx_string_put (data->win->mlx_ptr, data->win->win_ptr, 10, 10, 0xFF0000, num_str);
    mlx_string_put (data->win->mlx_ptr, data->win->win_ptr, 5, TRACKER_Y + TRACKER_H, 0x0, num_str);
}

static void draw_background(t_data *data) {
    // mlx_put_image_to_window(
    //     data->win->mlx_ptr,
    //     data->win->win_ptr,
    //     data->bg->img_ptr, 0, 0);
    // Combining the BG with foreground
}

static int update(void * p) {
    t_data  *data;
    t_img   *img_interpolated;

    data = (t_data*)p;
    img_interpolated = get_interpolation(data);

    unsigned img_x = (data->win->width / 2) - (img_interpolated->w / 2);
    unsigned img_y = (data->win->height / 2) - (img_interpolated->h / 2);
    draw_background(data);
    draw_foreground(data);
    mlx_put_image_to_window(
        data->win->mlx_ptr,
        data->win->win_ptr,
        img_interpolated->img_ptr, img_x, img_y);
    mlx_destroy_image(data->win->mlx_ptr, img_interpolated->img_ptr);
    free(img_interpolated);
    data->current_iteration++;
    data->current_iteration %= data->total_iterations;
    if (data->current_iteration == 0) {
        t_img * tmp = data->im1;
        data->im1 = data->im2;
        data->im2 = tmp;
    }
}

int ft_exit(void * ptr) {
    t_data *data = (t_data *)ptr;
    if (!data)
        exit(EXIT_FAILURE);
    mlx_destroy_image (data->win->mlx_ptr, data->bg->img_ptr);
    free(data->bg);
    mlx_destroy_image (data->win->mlx_ptr, data->fg->img_ptr);
    free(data->fg);
    mlx_destroy_image (data->win->mlx_ptr, data->im1->img_ptr);
    free(data->im1);
    mlx_destroy_image (data->win->mlx_ptr, data->im2->img_ptr);
    free(data->im2);
    exit(EXIT_SUCCESS);
}

int main(void)
{
    t_win win;
    t_data game_data;

    win = new_window(WIDTH, HEIGHT);
    if (!win.win_ptr)
        return (EXIT_FAILURE);
    game_data = new_game_data(&win);

    mlx_loop_hook(win.mlx_ptr, update, &game_data);
    mlx_hook(win.win_ptr, 17, 0, ft_exit, &game_data);
    mlx_loop(win.mlx_ptr);
    mlx_destroy_window(win.mlx_ptr, win.win_ptr);
    return (0);
}
