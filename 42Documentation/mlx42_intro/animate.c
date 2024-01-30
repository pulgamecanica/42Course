#include "animate.h"

void error(void);
int32_t mlx_get_pixel(mlx_image_t* image, uint32_t x, uint32_t y);

t_sprite new_sprite(char* file_path, mlx_t* mlx) {
  mlx_image_t* img;
  mlx_texture_t* texture;

  // Load png
  texture = mlx_load_png(file_path);
  if (!texture)
    error();

  // Create image from png texture
  img = mlx_texture_to_image(mlx, texture);
  if (!img)
    error();

  mlx_delete_texture(texture);

  return (t_sprite){img, mlx};
}

void    destroy_sprite(t_sprite* s) {
    if (!s)
        error();
    mlx_delete_image(s->mlx, s->sprite_img);
}

static void add_frame(t_animation* a, t_sprite* s, sprite_slice slice, int mirrored) {
    mlx_image_t* frame;

    frame = mlx_new_image(s->mlx, slice.width - (slice.padding_x * 2), slice.height - (slice.padding_y * 2));
    if (!frame)
        error();
    for (int i = 0; i < slice.height - (slice.padding_y * 2); i++) {
        for (int j = 0; j < slice.width - (slice.padding_x * 2); j++) {
            if (mirrored)
                mlx_put_pixel(frame, (slice.height - (slice.padding_y * 2)) - j - 1, i, mlx_get_pixel(s->sprite_img, slice.x + j + slice.padding_x, slice.y + i + slice.padding_y));
            else
                mlx_put_pixel(frame, j, i, mlx_get_pixel(s->sprite_img, slice.x + j + slice.padding_x, slice.y + i + slice.padding_y));
        }
    }
    ft_lstadd_back(&a->frames, ft_lstnew(frame));
}

t_animation* slice_sprite(t_sprite* s, sprite_slice slice, int mirrored, int frames, int frame_speed) {
    t_animation *   a;

    a = (t_animation*)calloc(sizeof(t_animation), 1);
    if (!a)
        error();
    *a = (t_animation){NULL, frame_speed, 0, 0, mirrored, 0, 0};
    for (int i = 0; i < frames; i++) {
        add_frame(a, s, slice, mirrored);
        slice.x += slice.width;
        if ((uint32_t)slice.x >= s->sprite_img->width) {
            slice.x = 0;
            slice.y += slice.height;
        }
    }
    return (a);
}

void update_animation(t_animation * a, double dt) {
  if (a) {
    a->accum += dt * 1000;
    if (a->accum > a->frame_speed) {
      a->accum -= a->frame_speed;
      a->current_frame_num++;
      a->current_frame_num %= ft_lstsize(a->frames);
    }
  }
}