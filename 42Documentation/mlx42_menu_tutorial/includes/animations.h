//***************************//
//*Template by pulgamecanica*//
//***************************//
#ifndef __ANIMATIONS_H__
# define __ANIMATIONS_H__

# include "libft.h"
# include "MLX42.h"

typedef struct s_animation {
    t_list * frames;
    int      width;
    int      height;
    int      delay;              // How many fps it takes to change animation
    int      _tmp_delay;         // Delay Iterator
    int      current_frame_num;  // Which frame is selected
    int      mirrored;
    long int updated_at;         // When was the last update
    long int frame_count;        // The frame count
    char *   name;
} t_animation;

typedef struct sprite_slice {
    int x;
    int y;
    int width;
    int height;
    int padding_x;
    int padding_y;
}   sprite_slice;

typedef struct parsed_slice {
    sprite_slice slice;
    char *       name;
    unsigned int frames;
    unsigned int delay;
} parsed_slice;

typedef struct s_sprite {
    t_list * animations;
    char   * name;
    char   * file_path;
    mlx_image_t * sprite_img;
    mlx_t * mlx;
} t_sprite;


/* ANIMATIONS */
void    update_animation(t_animation * a);
void    destroy_animation(void * ptr, void * mlx);

/* SPRITES */
t_sprite            new_sprite(char * name, char * file_path, mlx_t * mlx);
t_animation *       slice_sprite(t_sprite * s, sprite_slice slice, int mirrored, int frames, int delay, char * name);
// t_sprite *          load_file_sprite(t_win * win, char * path);
void                destroy_sprite(t_sprite * s);

#endif