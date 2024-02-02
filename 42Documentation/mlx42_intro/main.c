#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "color_game.h"
#include "dinos.h"
#include <math.h>

void bait(void*ptr){(void)ptr;};
void error(void) {
  puts(mlx_strerror(mlx_errno));
  exit(EXIT_FAILURE);
}

static mlx_image_t* my_load_png(char* file_path, mlx_t* mlx) {
  mlx_image_t*    img;
  mlx_texture_t*  texture;
  // Try to load the file
  texture = mlx_load_png(file_path);
  if (!texture)
    error();
  // Convert texture to a displayable image
  img = mlx_texture_to_image(mlx, texture);
  if (!img)
    error();
  if (mlx_image_to_window(mlx, img, 0, 0) == -1)
    error();
  mlx_delete_texture(texture);
  return (img);
}

t_color_game init_game() {
  mlx_t*          mlx;
  mlx_image_t*    img;
  mlx_image_t*    img2;
  mlx_image_t*    img3;
  mlx_image_t*    img4;
  mlx_image_t*    foreground_img;
  mlx_image_t*    difficulty_imgs[DIFFICULTY_LEN];
  t_animation*    anim;
  t_animation*    small_anim;
  t_animation*    star_anim;
  t_sprite        sprite;

  // Start mlx
  mlx = mlx_init(WIDTH, HEIGHT, "Color Game", false);
  if (!mlx)
    error();
  // Load the Background and Menu images
  img = my_load_png("./images/menu_bg.png", mlx);
  img2 = my_load_png("./images/game_bg.png", mlx);
  img3 = my_load_png("./images/score_bg.png", mlx);
  img4 = my_load_png("./images/lock.png", mlx);
  img2->instances[0].enabled = false; // disable game background for now
  img3->instances[0].enabled = false; // disable score background for now
  img4->instances[0].enabled = false; // disable score background for now
  {
    // Change lock color to something nicer...
    for (int i = 0; i < img4->width; i++) {
      for (int j = 0; j < img4->height; j++) {
        if (mlx_get_pixel(img4, i, j) != 0)
          mlx_put_pixel(img4, i, j, 0xBFCF34FF);
      }
    }
  }
  difficulty_imgs[0] = my_load_png("./images/menu_easy.png", mlx);
  difficulty_imgs[1] = my_load_png("./images/menu_medium.png", mlx);
  difficulty_imgs[2] = my_load_png("./images/menu_hard.png", mlx);
  // Sprite & Animation (menu select)
  sprite = new_sprite("./images/select_sprite_sheet.png", mlx);
  anim = slice_sprite(&sprite, (sprite_slice){0, 0, 128, 32, 0, 0}, false, 5, 120);
  destroy_sprite(&sprite);
  // Sprite & Animation (small select)
  sprite = new_sprite("./images/small_select_sprite_sheet.png", mlx);
  small_anim = slice_sprite(&sprite, (sprite_slice){0, 0, 32, 32, 0, 0}, false, 4, 120);
  destroy_sprite(&sprite);
  // Star animation
  sprite = new_sprite("./images/star.png", mlx);
  star_anim = slice_sprite(&sprite, (sprite_slice){0, 0, 32, 32, 0, 0}, false, 13, 320);
  destroy_sprite(&sprite);
  // Foreground
  foreground_img = mlx_new_image(mlx, WIDTH, HEIGHT);
  if (!foreground_img)
    error();
  return (t_color_game){mlx, img, img2, img3, foreground_img, img4,
    {difficulty_imgs[0], difficulty_imgs[1], difficulty_imgs[2]},
    anim, small_anim, star_anim, NULL, SELECT_PLAY, MENU, EASY, RED,
    {rand() % 256, rand() % 256, rand() % 256},
    {rand() % 256, rand() % 256, rand() % 255}, -1};
}

void my_mlx_put_string(char * str, t_color_game * cg, int x, int y) {
  mlx_image_t* tmp_text;

  tmp_text = mlx_put_string(cg->mlx, str, 0, 0);
  for (int i = 0; i < tmp_text->width; i++) {
    for (int j = 0; j < tmp_text->height; j++) {
      if (mlx_get_pixel(tmp_text, i, j) != 0)
        mlx_put_pixel(tmp_text, i, j, GAME_TEXT_COLOR);
    }
  }
  put_img_to_img(cg->foreground, tmp_text, x, y);
  mlx_delete_image(cg->mlx, tmp_text);
}

static double colors_dist(int r1, int g1, int b1, int r2, int g2, int b2) {
  return (sqrt(
    pow((r2 - r1), 2) +
    pow((g2 - g1), 2) +
    pow((b2 - b1), 2)));
}

static int colors_diff(int r1, int g1, int b1, int r2, int g2, int b2) {
  double d = colors_dist(r1, g1, b1, r2, g2, b2);
  return 100 - (d * 100) / 441.672956;
}

void update(void* ptr) {
  static int fps;
  static int menu_selection_coords[SELECTION_LEN][2] = {{256, 160}, {256, 256}};
  static int color_selection_coords[COLOR_SELECTION_LEN][2] = {{320, 256}, {416, 256}, {512, 256}};
  int user_color, game_color;

  t_color_game* cg = (t_color_game*)ptr;

  fps = 1000 / cg->mlx->delta_time;
  printf("\e[1;1H\e[2Jfps [%d]\n\n", fps);
  // Clean the foreground
  memset(cg->foreground->pixels, 0xFF000000, cg->foreground->width * cg->foreground->height * BPP);
  user_color = get_rgba(cg->selected_colors[RED], cg->selected_colors[GREEN], cg->selected_colors[BLUE], 255);
  game_color = get_rgba(cg->game_colors[RED], cg->game_colors[GREEN], cg->game_colors[BLUE], 255);
  if (cg->game_status == SCORE) {
    // Update the dinos!
    ft_lstiter_param(cg->random_dinos, update_dinos, cg);
    // Update the score if needed
    if (cg->score < 0) {
      cg->score = colors_diff(cg->selected_colors[RED], cg->selected_colors[GREEN], cg->selected_colors[BLUE],
        cg->game_colors[RED], cg->game_colors[GREEN], cg->game_colors[BLUE]);
    }

    {
      // Calculate the stars needed
      mlx_image_t * frame = (mlx_image_t *)ft_lstget(cg->star_animation->frames, cg->star_animation->current_frame_num)->content;
      if (!frame)
        error();
      if (cg->score > 50)
        put_img_to_img(cg->foreground, frame, (WIDTH / 2) - 48, 100);
      if (cg->score > 90)
        put_img_to_img(cg->foreground, frame, (WIDTH / 2) - 14, 80);
      if (cg->score > 96)
        put_img_to_img(cg->foreground, frame, (WIDTH / 2) + 16, 100);

      update_animation(cg->star_animation, cg->mlx->delta_time);
    }
    {
      // Put the score to the screen
      char buf[5] = {0};
      sprintf(buf, "%d%%", cg->score);
      my_mlx_put_string(buf, cg, 385, 120);
    }

    {
      // Put user hexa color 
      char buf1[11] = {0};
      char buf2[7] = {0};
      sprintf(buf1, "%X", user_color);
      ft_strlcat(buf2, buf1, 7);
      my_mlx_put_string(buf2, cg, 210, 200);
      draw_circle(cg->foreground, 200, 209, 6, user_color);
    }
    {
      // Put game hexa color 
      char buf1[11] = {0};
      char buf2[7] = {0};
      sprintf(buf1, "%X", game_color);
      ft_strlcat(buf2, buf1, 7);
      my_mlx_put_string(buf2, cg, 370, 200);
      draw_circle(cg->foreground, 360, 209, 6, game_color);
    }
    {
      // Draw User Red, Green & Blue
      int coords[6][2] = {
        {194, 260}, {224, 260}, {258, 260},
        {354, 260}, {384, 260}, {418, 260},
      };
      for (int i = 0; i < 3; i++) {
        char buf1[4] = {0};
        char buf2[4] = {0};
        sprintf(buf1, "%d", cg->selected_colors[i]);
        my_mlx_put_string(buf1, cg, coords[i][0], coords[i][1]);
        sprintf(buf2, "%d", cg->game_colors[i]);
        my_mlx_put_string(buf2, cg, coords[i + 3][0], coords[i + 3][1]);
      }
    }

  } else if (cg->game_status == PLAYING) {
    // Draw the lock on the selection if needed  
    if (cg->game_difficulty == EASY || cg->game_difficulty == MEDIUM) {
      put_img_to_img(cg->foreground, cg->lock_img,
        color_selection_coords[2][0],
        color_selection_coords[2][1] + 8);
    }
    if (cg->game_difficulty == EASY) {
      put_img_to_img(cg->foreground, cg->lock_img,
        color_selection_coords[1][0],
        color_selection_coords[1][1] + 8);
    }
    // Put the number (amount) of each color selection
    for (int n = 0; n < COLOR_SELECTION_LEN; n++) {
      char * num_str = ft_itoa(cg->selected_colors[n]);
      int padding_left = 0;
      if (cg->selected_colors[n] < 100)
        padding_left += 6;
      if (cg->selected_colors[n] < 9)
        padding_left += 5;
      my_mlx_put_string(num_str, cg, color_selection_coords[n][0] + padding_left, color_selection_coords[n][1] + 5);
      free(num_str);
    }
    // Draw the circles
    draw_circle(cg->foreground, (WIDTH / 20) * 13.5, (HEIGHT / 5) * 2, 72, 0xFF);
    draw_circle(cg->foreground, (WIDTH / 20) * 13.5, (HEIGHT / 5) * 2, 70, game_color);
    draw_circle(cg->foreground, (WIDTH / 20) * 13.5, (HEIGHT / 5) * 2, 42, user_color);
    // Draw the color selection animation
    mlx_image_t * frame = (mlx_image_t *)ft_lstget(cg->small_select_animation->frames, cg->small_select_animation->current_frame_num)->content;
    if (!frame)
      error();
    put_img_to_img(cg->foreground, frame,
      color_selection_coords[cg->color_selection][0],
      color_selection_coords[cg->color_selection][1]);
    update_animation(cg->small_select_animation, cg->mlx->delta_time);
  } else if (cg->game_status == MENU) {
    // Paint the select animation on the foreground
    mlx_image_t * frame = (mlx_image_t *)ft_lstget(cg->select_animation->frames, cg->select_animation->current_frame_num)->content;
    if (!frame)
      error();
    put_img_to_img(cg->foreground, frame,
      menu_selection_coords[cg->menu_selection][0],
      menu_selection_coords[cg->menu_selection][1]);
    update_animation(cg->select_animation, cg->mlx->delta_time);
    // Draw the dificulty image to the menu (Easy, Medium or Hard)
    for (int i = 0; i < DIFFICULTY_LEN; i++) {
      cg->difficulty_imgs[i]->instances[0].enabled = false;
    }
    cg->difficulty_imgs[cg->game_difficulty]->instances[0].enabled = true;
    // Logic for the menu
    if (mlx_is_key_down(cg->mlx, MLX_KEY_DOWN)) {
      cg->menu_selection = SELECT_DIFFICULTY;
    }
    if (mlx_is_key_down(cg->mlx, MLX_KEY_UP)) {
      cg->menu_selection = SELECT_PLAY;
    }
  }
}

void key_update(mlx_key_data_t keydata, void* ptr) {
  t_color_game* cg = (t_color_game*)ptr;

  if (cg->game_status == PLAYING && (keydata.action == MLX_REPEAT || keydata.action == MLX_PRESS)) {
    if (keydata.key == MLX_KEY_ENTER) {
      cg->game_bg->instances[0].enabled = false;
      cg->score_bg->instances[0].enabled = true;
      cg->game_status = SCORE;
    } else if (keydata.key == MLX_KEY_DOWN) {
      if (cg->selected_colors[cg->color_selection] > 0)
        cg->selected_colors[cg->color_selection]--;
    } else if (keydata.key == MLX_KEY_UP) {
      if (cg->selected_colors[cg->color_selection] < 255)
        cg->selected_colors[cg->color_selection]++;
    }
  }
  if (cg->game_status == PLAYING && keydata.action == MLX_PRESS) {
    int available_colors = COLOR_SELECTION_LEN;
    if (cg->game_difficulty == EASY)
      available_colors = 1;
    else if (cg->game_difficulty == MEDIUM)
      available_colors = 2;
    if (keydata.key == MLX_KEY_LEFT) {
      cg->color_selection--;
      if (cg->color_selection == -1)
        cg->color_selection = available_colors - 1;
    } else if (keydata.key == MLX_KEY_RIGHT) {
      cg->color_selection++;
      cg->color_selection %= available_colors;
    }
  } else if (cg->game_status == MENU && keydata.action == MLX_RELEASE) {
    if (keydata.key == MLX_KEY_ENTER) {
      if (cg->game_difficulty == EASY) {
        cg->selected_colors[GREEN] = cg->game_colors[GREEN];
        cg->selected_colors[BLUE] = cg->game_colors[BLUE];
      } else if (cg->game_difficulty == MEDIUM) {
        cg->selected_colors[BLUE] = cg->game_colors[BLUE];
      }
      for (int i = 0; i < DIFFICULTY_LEN; i++)
        cg->difficulty_imgs[i]->instances[0].enabled = false;
      cg->menu_bg->instances[0].enabled = false;
      cg->game_bg->instances[0].enabled = true;
      cg->game_status = PLAYING;
    }
  } else if (cg->game_status == MENU && cg->menu_selection == SELECT_DIFFICULTY && keydata.action == MLX_PRESS) {
    if (keydata.key == MLX_KEY_LEFT) {
      cg->game_difficulty--;
      if (cg->game_difficulty == -1)
        cg->game_difficulty = DIFFICULTY_LEN - 1;
    } else if (keydata.key == MLX_KEY_RIGHT) {
      cg->game_difficulty++;
      cg->game_difficulty %= DIFFICULTY_LEN;
    }
  }
}


void init_dinos(t_color_game * cg) {
  ft_lstadd_back(&cg->random_dinos, ft_lstnew(create_dino("./images/dino_doux.png", cg->mlx)));
  ft_lstadd_back(&cg->random_dinos, ft_lstnew(create_dino("./images/dino_mort.png", cg->mlx)));
  ft_lstadd_back(&cg->random_dinos, ft_lstnew(create_dino("./images/dino_tard.png", cg->mlx)));
  ft_lstadd_back(&cg->random_dinos, ft_lstnew(create_dino("./images/dino_vita.png", cg->mlx)));
}

#include <time.h>

int32_t main(void) {
  srand( time( NULL ) );
  t_color_game cg;

  cg = init_game();
  init_dinos(&cg);

  if (mlx_image_to_window(cg.mlx, cg.foreground, 0, 0) == -1)
    error();

  mlx_loop_hook(cg.mlx, update, &cg);
  mlx_key_hook(cg.mlx, key_update, &cg);
  mlx_loop(cg.mlx);

  ft_lstclear(&cg.select_animation->frames, bait);
  free(cg.select_animation);
  ft_lstclear(&cg.small_select_animation->frames, bait);
  free(cg.small_select_animation);
  ft_lstclear(&cg.star_animation->frames, bait);
  free(cg.star_animation);
  // Free random dinos
  ft_lstclear(&cg.random_dinos, destroy_dino);
  free(cg.random_dinos);

  mlx_terminate(cg.mlx);
  return (EXIT_SUCCESS);
}