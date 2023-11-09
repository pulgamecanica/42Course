# This library is going to help you generate animations

### An animation is a compilation of images that run in a sequence in a certain amount of time
### A frame is the name given to each image which composes an animation
### A sprite sheet is a canvas with many frames, there can be more than one animation in a sprite sheet

A sprite sheet might look something like this:

> A1 A2 A3 
> A4 B1 B2
> B3 B4 B5
> B6 B7

##### Where each `Ax` is a frame of the animation `A`, same goes for `B`

### FUNCTIONS

```c
// utils mlx animations
t_win					new_window(int w, int h, char *str);
t_img					new_file_img(char * path, t_win window);
t_img					new_img(int w, int h, t_win window);
unsigned int	get_pixel_img(t_img img, int x, int y);
void					put_pixel_img(t_img img, int x, int y, int color);
void					put_img_to_img(t_img dst, t_img src, int x, int y);
void					destroy_image(t_img img);
void					destroy_window(t_win w);
void					free_list(char ** list);
int						ft_list_size(char ** list);
char	*				get_next_line_nl(int fd);

// sprite
t_sprite		  new_sprite(char * name, char * file_path, t_win * win);
t_animation	* slice_sprite(t_sprite s, sprite_slice slice, int mirrored, int frames, int delay, char * name);
t_sprite		*	load_file_sprite(t_win * win, char * path);
void          destroy_sprite(t_sprite s);

// structs
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

typedef struct s_sprite {
	t_list * animations;
	char   * name;
	char   * file_path;
	t_img    sprite_img;
	int      width;
	int      height;
	int      z_index;
} t_sprite;

typedef	struct sprite_slice {
	int x;
	int y;
	int width;
	int height;
}	sprite_slice;

typedef struct parsed_slice {
	sprite_slice slice;
	char *       name;
	unsigned int frames;
	unsigned int delay;
} parsed_slice;

```