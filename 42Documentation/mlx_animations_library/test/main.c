#include "utils.h"
#include "sprite.h"
#include "libft.h"
#include "colors.h"
#include <sys/time.h>

typedef struct s_cord {
  int x;
  int y;
} t_cord;

typedef struct s_demo {
  t_win   * tutorial;
  t_img   * img_bg;
  t_img   * img_canvas;
  t_list  * sprites;
  double    scale;
  t_cord    pos;
  int       fps;
  uint64_t created_at;
  uint64_t updated_at;
} t_demo;

uint64_t  gettimeofday_ms(void) {
  static struct timeval tv;

  gettimeofday(&tv, NULL);
  return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

uint64_t  timestamp_in_ms(t_demo * demo) {
  return (gettimeofday_ms() - demo->created_at);
}

void destroy_sprite_helper(void * ptr) {
  t_sprite * s;

  s = (t_sprite *)ptr;
  if (s) {
    destroy_sprite(*s);
    free(s);
  }
}

void put_all_sprites_to_window(t_win * win, t_img * img, t_list * sprites, double scale, t_cord * _pos) {
  t_cord        pos;
  t_sprite *    sprite;
  t_list  *     animations;
  t_animation * animation;
  t_img *       frame;

  pos = *_pos;
  while (sprites) {
    sprite = (t_sprite *)(sprites->content);
    if (!sprite)
      return ;

    animations = sprite->animations;
    if (!animations)
      return ;
    while (animations) {

      animation = (t_animation *)animations->content;
      if (!animation)
        return ;
      frame = (t_img *)ft_lstget(animation->frames, animation->current_frame_num)->content;
      if (!frame)
        return ;

      put_img_to_img_scale(img, frame, pos.x, pos.y, scale);
      update_animation(animation);
      pos.x += frame->w * scale;
      if (pos.x > 800 - (frame->w * scale)) {
        pos.x = 0;
        pos.y += frame->h * scale;
      }
      animations = animations->next;
    }
    pos.x = _pos->x;
    pos.y += frame->h ? (frame->h * scale) : 64;
    sprites = sprites->next;
  }
}

static int update(t_demo * demo) {
  t_img   * img_bg;
  t_img   * img_canvas;
  t_list  * sprites;

  if (timestamp_in_ms(demo) - demo->updated_at < (uint64_t)(1000 / demo->fps)) {
    // printf("\033c%dFPS [%ld], no_update_time: [%ld]\n", demo->fps, timestamp_in_ms(demo), timestamp_in_ms(demo) - demo->updated_at);
    return 0;
  }
  img_canvas = demo->img_canvas;
  img_bg = demo->img_bg;
  sprites = demo->sprites;

  put_img_to_img(img_canvas, img_bg, 0, 0);
  put_all_sprites_to_window(demo->tutorial, img_canvas, sprites, demo->scale, &demo->pos);
  mlx_put_image_to_window(demo->tutorial->mlx_ptr, demo->tutorial->win_ptr, img_canvas->img_ptr, 0, 0);
  demo->updated_at = timestamp_in_ms(demo);

  return 1;
}

int key_pressed(int key, void *param) {
  t_demo *  demo;

  demo = (t_demo *)param; 
  if (key == 93 && demo->scale < 4) // Zoom in
    demo->scale += 0.1;
  else if (key == 47 && demo->scale > 0.5) // Zoom out
    demo->scale -= 0.1;
  else if (key == 65363 && demo->pos.x < demo->img_canvas->w) // Left
    demo->pos.x += 5;
  else if (key == 65361 && demo->pos.x > 0) // Right
    demo->pos.x -= 5;
  else if (key == 65362) // Up
    demo->pos.y -= 5;
  else if (key == 65364 && demo->pos.y < demo->img_canvas->h) // Down
    demo->pos.y += 5;
  else if (key == 48) // Center (Zero)
    demo->pos = (t_cord){0, 0};
  return 0;
}

int ft_exit(void *param) {
  t_demo *  demo;

  demo = (t_demo *)param;
  if (!demo)
    return -1;
  destroy_image(demo->img_bg);
  destroy_image(demo->img_canvas);
  ft_lstclear(&demo->sprites, destroy_sprite_helper);
  destroy_window(demo->tutorial);
  exit(0);
}

int main(int ac, char *av[]) {
  t_demo     demo;
  t_win    * tutorial;
  t_img    * img_bg;
  t_img    * img_canvas;
  t_list  ** sprites;
  int        i;

  tutorial = new_window(800, 390, "animations");
  if (!tutorial)
    return 3;
  
  img_bg = new_file_img("assets/bg.xpm", tutorial);
  if (!img_bg)
    return 3;
  img_canvas = new_img(img_bg->w, img_bg->h, tutorial);
  if (!img_canvas)
    return 3;
  
  sprites = (t_list **)ft_calloc(sizeof(t_list *), 1);
  if (!sprites)
    return 2;
  i = 1;
  while (i < ac) {
    ft_lstadd_back(sprites, ft_lstnew(load_file_sprite(tutorial, av[i++])));
  }
  
  demo = (t_demo){tutorial, img_bg, img_canvas, *sprites, 1.0, (t_cord){0, 0}, 32, gettimeofday_ms(), gettimeofday_ms()};

  mlx_loop_hook(tutorial->mlx_ptr, update, &demo);
  mlx_hook(tutorial->win_ptr, 2, 1L<<0,  key_pressed, &demo);
  mlx_hook(tutorial->win_ptr, 17, 0, ft_exit, &demo);
  mlx_loop(tutorial->mlx_ptr);
  // destroy_image(img_bg);
  // destroy_image(img_canvas);
  // ft_lstclear(sprites, destroy_sprite_helper);
  // destroy_window(tutorial);
  return (0);
}