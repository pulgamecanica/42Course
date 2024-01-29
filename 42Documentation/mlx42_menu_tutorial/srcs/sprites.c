#include "animations.h"
#include "menu_tutorial.h"

t_sprite    new_sprite(char * name, char * file_path, mlx_t * mlx) {
    mlx_image_t *   img;
    mlx_texture_t * texture;

    // Load png
    texture = mlx_load_png(file_path);
    if (!texture)
        exit(EXIT_FAILURE);

    // Create image from png texture
    img = mlx_texture_to_image(mlx, texture);
    if (!img)
        exit (2);
    mlx_delete_texture(texture);
    return (t_sprite){NULL, ft_strdup(name),
        ft_strdup(file_path), img, mlx};
}

void    destroy_sprite(t_sprite * s) {
    if (!s)
        exit(EXIT_FAILURE);
    printf("Animation[%s] # of animations[%d]\n", s->name, ft_lstsize(s->animations));
    free(s->name);
    free(s->file_path);
    ft_lstiter_param(s->animations, destroy_animation, s->mlx);
    // ft_lstclear(&s->animations, free);
    free(s->animations);
    mlx_delete_image(s->mlx, s->sprite_img);
}

static void add_frame(t_animation * a, t_sprite * s, sprite_slice slice, int mirrored) {
    mlx_image_t * frame;
    int         i;
    int         j;

    frame = mlx_new_image(s->mlx, slice.width - (slice.padding_x * 2), slice.height - (slice.padding_y * 2));
    if (!frame)
        exit(EXIT_FAILURE);
    i = 0;
    while (i < slice.height - (slice.padding_y * 2)) {
        j = 0;
        while (j < slice.width - (slice.padding_x * 2)) {
            if (mirrored)
                mlx_put_pixel(frame, (slice.height - (slice.padding_y * 2)) - j - 1, i, mlx_get_pixel(s->sprite_img, slice.x + j + slice.padding_x, slice.y + i + slice.padding_y));
            else
                mlx_put_pixel(frame, j, i, mlx_get_pixel(s->sprite_img, slice.x + j + slice.padding_x, slice.y + i + slice.padding_y));
            j++;
        }
        i++;
    }
    ft_lstadd_back(&a->frames, ft_lstnew(frame));
}

t_animation *   slice_sprite(t_sprite * s, sprite_slice slice, int mirrored, int frames, int delay, char * name) {
    t_animation *   a;
    int                     i;

    a = (t_animation *)calloc(sizeof(t_animation), 1);
    if (!a)
        exit (2);
    *a = (t_animation){NULL, slice.width, slice.height, delay, 0, 0, mirrored, 0, 0, ft_strdup(name)};
    i = 0;
    while (i < frames) {
        add_frame(a, s, slice, mirrored);
        slice.x += slice.width;
        if ((uint32_t)slice.x >= s->sprite_img->width) {
            slice.x = 0;
            slice.y += slice.height;
        }
        i++;
    }
    return a;
}