#include "dinos.h"
#include "color_game.h"

static void destroy_animation(void* ptr) {
  t_animation* a = (t_animation*)ptr;

  ft_lstclear(&a->frames, bait);
  free(a);
}

void update_dinos(void* ptr1, void* ptr2) {
  t_dino* dino = (t_dino*) ptr1;
  t_color_game* cg = (t_color_game*) ptr2;
  t_animation * action_animation;
  int dino_action_index;

  dino_action_index = (dino->dino_action - 1) + (4 * dino->mirrored);
  action_animation = (t_animation *)ft_lstget(dino->actions, dino_action_index)->content;

  mlx_image_t * frame = (mlx_image_t *)ft_lstget(action_animation->frames, action_animation->current_frame_num)->content;
  if (!frame)
    error();
  put_img_to_img(cg->foreground, frame, dino->x, dino->y);
  update_animation(action_animation, cg->mlx->delta_time);

  // Change direction 0.5% probable
  if (rand() % 1000 < 5)
    dino->mirrored = !dino->mirrored;
  // Change dino action 1% probable
  if (rand() % 100 < 1)
    dino->dino_action = 1 + (rand() % 4);
  // Update dino movement 50% probable
  if (dino->dino_action == RUNNING || dino->dino_action == JUMPING) {
    if (rand() % 2) {
      if (dino->mirrored)
        dino->x--;
      else
        dino->x++;
      if (dino->x < 0)
        dino->x == WIDTH - 24;
      dino->x %= WIDTH;
    }
  }
}

t_dino * create_dino(char* file_path, mlx_t* mlx) {
  t_sprite sprite;
  t_animation * animation;
  t_dino* dino;

  sprite_slice slices[4] = {
    (sprite_slice){0, 0, 24, 24, 0, 0},       // IDLE [4 frames]
    (sprite_slice){4 * 24, 0, 24, 24, 0, 0},  // JUMP [4 frames]
    (sprite_slice){13 * 24, 0, 24, 24, 0, 0}, // EXPLODE [4 frames]
    (sprite_slice){17 * 24, 0, 24, 24, 0, 0}  // RUN [5 frames]
  };
  int num_frames[4] = {4, 4, 4, 5};

  dino = (t_dino *)calloc(sizeof(t_dino), 1);
  if (!dino)
    error();
  // Choose Random starting dino action
  dino->dino_action = 1 + (rand() % 4);
  // Choose random dino spawn
  dino->y = HEIGHT - 24 - (rand() % 10);
  dino->x = rand() % WIDTH;
  // Load sprite and cut all the animations
  sprite = new_sprite(file_path, mlx);
  for (int i = 0; i < 8; i++) {
    bool mirrored = i >= 4;
    animation = slice_sprite(&sprite, slices[i % 4], mirrored, num_frames[i % 4], 300);
    ft_lstadd_back(&dino->actions, ft_lstnew(animation));
  }  
  destroy_sprite(&sprite);
  return dino;
}


void destroy_dino(void* ptr) {
  t_dino * dino = (t_dino*)ptr;

  ft_lstclear(&dino->actions, destroy_animation);
  free(dino);
}