#ifndef __DINOS_H__
# define __DINOS_H__

#include <stdbool.h>
#include "MLX42/MLX42.h"
#include "libft.h"

enum dino_action {
  IDLE = 1,
  RUNNING = 2,
  EXPLODING = 3,
  JUMPING = 4
};

typedef struct s_dino {
  int x;
  int y;
  enum dino_action dino_action;
  bool mirrored;
  t_list * actions; //  List of all the dino actions, 8 total animations
} t_dino;

void destroy_dino(void* ptr);
void update_dinos(void* ptr1, void* ptr2);
t_dino * create_dino(char* file_path, mlx_t* mlx);

#endif
