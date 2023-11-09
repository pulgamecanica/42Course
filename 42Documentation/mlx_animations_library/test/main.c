#include "utils.h"
#include "sprite.h"
#include "libft.h"

typedef struct s_demo {
  t_win     tutorial;
  t_img     img_bg;
  t_img     img_canvas;
  t_list  * sprites;

  // t_animation * current_animation;
  // int x;
  // int y;

} t_demo;

void destroy_sprite_helper(void * ptr) {
  t_sprite * s;

  s = (t_sprite *)ptr;
  if (s) {
    destroy_sprite(*s);
    free(s);
  }
}

void put_all_sprites_to_window(t_win * win, t_img * img, t_list * sprites) {
  t_sprite * sprite;

  sprite = (t_sprite *)ft_lstlast(sprites)->content;

  // put_img_to_img(img, &sprite->sprite_img, 0, 0);

  t_list * animations;
  animations = sprite->animations;

  t_animation * animation;
  animation = (t_animation *)ft_lstget(animations, 3)->content;



  t_img * frame;
  frame = (t_img *)ft_lstget(animation->frames, animation->current_frame_num)->content;

  put_img_to_img(img, frame, (img->w / 2) - (frame->w / 2), (img->h / 2) - (frame->h / 2));

  update_animation(animation);
}

static int update(t_demo * demo) {
  t_win     tutorial;
  t_img     img_bg;
  t_img     img_canvas;
  t_list  * sprites;

  tutorial = demo->tutorial;
  img_canvas = demo->img_canvas;
  img_bg = demo->img_bg;
  sprites = demo->sprites;

  put_img_to_img(&img_canvas, &img_bg, 0, 0);
  put_all_sprites_to_window(&tutorial, &img_canvas, sprites);
  mlx_put_image_to_window(tutorial.mlx_ptr, tutorial.win_ptr, img_canvas.img_ptr, 0, 0);
}

int main(int ac, char *av[]) {
  t_demo    demo;
  t_win     tutorial;
  t_img     img_bg;
  t_img     img_canvas;
  t_list  * sprites;
  int       i;

  tutorial = new_window(626, 360, "animations");
  if (!tutorial.win_ptr)
    return 3;
  
  img_bg = new_file_img("assets/bg.xpm", tutorial);
  if (!img_bg.img_ptr)
    return 3;
  img_canvas = new_img(img_bg.w, img_bg.h, tutorial);
  if (!img_canvas.img_ptr)
    return 3;
  
  sprites = (t_list *)ft_calloc(sizeof(t_list), 1);
  if (!sprites)
    return 2;
  i = 1;
  while (i < ac) {
    ft_lstadd_back(&sprites, ft_lstnew(load_file_sprite(&tutorial, av[i++])));
  }
  
  demo = (t_demo){tutorial, img_bg, img_canvas, sprites};

  mlx_loop_hook(tutorial.mlx_ptr, update, &demo);

  mlx_loop(tutorial.mlx_ptr);
  destroy_window(tutorial);
  ft_lstclear(&sprites, destroy_sprite_helper);
  return (0);
}