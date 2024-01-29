// -----------------------------------------------------------------------------
// Codam Coding College, Amsterdam @ 2022-2023 by W2Wizard.
// See README in the root project for more information.
// -----------------------------------------------------------------------------

#include "menu_tutorial.h"
#include "animations.h"

#define WIDTH 512
#define HEIGHT 512
#define POINTS 8
#define RADIUS 4

const uint32_t RED = 0xFF0000FF;
const uint32_t GREEN = 0x00FF00FF;
const uint32_t BLUE = 0x0000FFFF;
const uint32_t GOLD = 0xFFD700FF;
const uint32_t YELLOW = 0xFFFF00FF;
const uint32_t PEACH = 0xFFE5B4FF;
const uint32_t PISTACHIO = 0x93C572FF;
const uint32_t TURQUOISE = 0xAFEEEEFF;
const uint32_t AQUAMARINE = 0x7FFFD4FF;
const uint32_t ORANGE = 0xFFAC1CFF;

typedef struct s_point {
	int x;
	int y;
	uint32_t color;
}	t_point;

typedef struct s_game {
	t_point path[POINTS];
	mlx_image_t * img_square;
	mlx_image_t * img_bg;
	mlx_image_t * img;
	mlx_t * mlx;
	t_animation * a;
	// mlx_image_t * bg;
}	t_game;

// -----------------------------------------------------------------------------

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a) {
    return (r << 24 | g << 16 | b << 8 | a);
}

void ft_randomize(void* param) {
	t_game * g = (t_game *)param;
	
	for (uint32_t i = 0; i < g->img_square->width; ++i)
	{
		for (uint32_t y = 0; y < g->img_square->height; ++y)
		{
			uint32_t color = ft_pixel(
				rand() % 0xFF, // R
				rand() % 0xFF, // G
				rand() % 0xFF, // B
				rand() % 0xFF  // A
			);
			mlx_put_pixel(g->img_square, i, y, color);
		}
	}
}

void ft_hook(void* param) {
	t_game * g = (t_game *)param;

	if (mlx_is_key_down(g->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(g->mlx);
	if (mlx_is_key_down(g->mlx, MLX_KEY_UP))
		g->img_square->instances[0].y -= 5;
	if (mlx_is_key_down(g->mlx, MLX_KEY_DOWN))
		g->img_square->instances[0].y += 5;
	if (mlx_is_key_down(g->mlx, MLX_KEY_LEFT))
		g->img_square->instances[0].x -= 5;
	if (mlx_is_key_down(g->mlx, MLX_KEY_RIGHT))
		g->img_square->instances[0].x += 5;
}

// -----------------------------------------------------------------------------


void draw_path_on_image(t_game * game) {
	for (uint32_t i = 0; i < POINTS; i++) {
		// draw_circle(game->img_path, game->path[i].x, game->path[i].y, RADIUS, game->path[i].color);
	    // draw_line(game->img_path, game->path[i].x, game->path[i].y, game->path[(i + 1) % POINTS].x, game->path[(i + 1) % POINTS].y, GREEN);
		
		int x[4] , y[4];
	    
	    // Controll points
	    x[0] =  game->path[i].x;
	    y[0] =  game->path[i].y;

	    x[1] =  game->path[i].x - 84;
	    y[1] =  game->path[i].y - 42;

	    x[2] =  game->path[(i + 1) % POINTS].x + 42;
	    y[2] =  game->path[(i + 1) % POINTS].y + 45;

	    x[3] =  game->path[(i + 1) % POINTS].x;
	    y[3] =  game->path[(i + 1) % POINTS].y;
		
	    bezier_curve(game->img, x, y, ORANGE);
	}
}

void draw_background(mlx_t * mlx, mlx_image_t * img) {
    char background_img[] = "./res/images/homemenu_bg.png";
    mlx_image_t *   bg_img;
    mlx_texture_t * texture;

    // Load png
    texture = mlx_load_png(background_img);
    if (!texture)
        return ;
    // Create image from png texture
    bg_img = mlx_texture_to_image(mlx, texture);
    if (!bg_img)
        return ;
    // Put image enabled to window
    for(uint32_t i = 0; i < bg_img->width && i < img->width; i++) {
        for (uint32_t j = 0; j < bg_img->height && i < img->height; j++) {
            uint32_t color = mlx_get_pixel(bg_img, i, j);
            mlx_put_pixel(img, i, j, color); 
        }
    }
    mlx_delete_texture(texture);
    mlx_delete_image(mlx, bg_img);
}


void update(void * param) {
	t_game * g = (t_game *)param;
	
	// draw_background(g->mlx, g->img);
	draw_path_on_image(g);
	ft_memset(g->img->pixels, 0xFF000000, g->img->width * g->img->height * 4);
	if (g->a) {
		mlx_image_t * frame = (mlx_image_t *)ft_lstget(g->a->frames, g->a->current_frame_num)->content;
		if (!frame)
			exit(EXIT_FAILURE);
		put_img_to_img_scale(g->img, frame, 50, 50, 1);
		update_animation(g->a);
	}
}

void bait(void *ptr) {(void)ptr;}

int32_t main(void) {
	mlx_t* mlx;
	mlx_image_t * image;
	mlx_image_t * image2;
	mlx_image_t * image3;

	// Gotta error check this stuff
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(image = mlx_new_image(mlx, 128, 128)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(image2 = mlx_new_image(mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(image3 = mlx_new_image(mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	// Load cacodemon sprite
	t_sprite s = new_sprite("Cacodemon", "./res/sprites/Cacodemon.png", mlx);
	// Load animation
	t_animation * a = slice_sprite(&s, (sprite_slice){0,0,64,64,0,0}, 0, 6, 8, "idle");
	destroy_sprite(&s);
	
	t_game g = (t_game){
		{
			(t_point){169, 48, RED}, (t_point){189, 269, RED}, (t_point){250, 46, RED}, (t_point){437, 238, RED}, (t_point){156, 284, RED}, (t_point){14, 274, RED}, (t_point){495, 379, RED}, (t_point){173, 208, RED}
		},
		image,
		image2,
		image3,
		mlx,
		a
	};

	draw_background(mlx, image2);
	// draw_path_on_image(&g);
	
	// mlx_image_t * frame = (mlx_image_t *)ft_lstget(a->frames, a->current_frame_num)->content;
	// if (!frame)
	// 	exit(EXIT_FAILURE);

	// put_img_to_img_scale(image2, frame, 50, 50, 1);
	// update_animation(a);
	mlx_loop_hook(mlx, update, &g);



	if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(mlx, image2, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(mlx, image3, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	// image2->instances[0].z = 1;
	// image3->instances[0].z = 2;
	
	mlx_loop_hook(mlx, ft_randomize, &g);
	mlx_loop_hook(mlx, ft_hook, &g);

	mlx_loop(mlx);

	ft_lstclear(&g.a->frames, bait);
	free(g.a->name);
	free(g.a);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
