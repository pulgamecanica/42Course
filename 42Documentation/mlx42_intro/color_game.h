#ifndef __COLOR_GAME_H__
# define __COLOR_GAME_H__

#include "MLX42/MLX42.h"
#include "animate.h"

#define WIDTH 640
#define HEIGHT 360
#define BPP sizeof(int32_t)
#define SELECTION_LEN 2
#define DIFFICULTY_LEN 3
#define COLOR_SELECTION_LEN 3
#define GAME_TEXT_COLOR 0x424242FF

void draw_circle(mlx_image_t * img, int xc, int yc, int r, uint32_t color);
int get_rgba(int r, int g, int b, int a);
int32_t mlx_get_pixel(mlx_image_t* image, uint32_t x, uint32_t y);
void error(void);
void bait(void*ptr);

enum game_status {
  MENU,
  PLAYING,
  SCORE
};

enum menu_selection {
  SELECT_PLAY,
  SELECT_DIFFICULTY
};

enum game_difficulty {
  EASY,
  MEDIUM,
  HARD
};

enum color_selection {
  RED,
  GREEN,
  BLUE
};

typedef struct s_color_game {
  mlx_t* mlx;
  mlx_image_t* menu_bg;
  mlx_image_t* game_bg;
  mlx_image_t* score_bg;
  mlx_image_t* foreground;
  mlx_image_t* lock_img;
  mlx_image_t* difficulty_imgs[DIFFICULTY_LEN];
  t_animation* select_animation;
  t_animation* small_select_animation;
  t_animation* star_animation;
  t_list*      random_dinos;
  enum menu_selection menu_selection;
  enum game_status game_status;
  enum game_difficulty game_difficulty;
  enum color_selection color_selection;
  int selected_colors[COLOR_SELECTION_LEN];
  int game_colors[COLOR_SELECTION_LEN];
  int score;
} t_color_game;

#endif
