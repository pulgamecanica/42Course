/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SnakeGame.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname- <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 4242/42/42 42:42:42 by somename-         #+#    #+#             */
/*   Updated: 4242/42/42 42:42:42 by somename-        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "canvas.h"
#include "game.h"

/*
draw_square - Draw each pixel of a square on the image
  Parameters:
    [square] Square that will be drawn
    [img]    Image where square will be drawn
*/
void	draw_square(t_square square, t_img img)
{
	unsigned short int	i;
	unsigned short int	j;

	i = 0;
	while (i < square.size && i + square.y < img.h)
	{
		j = 0;
		while (j < square.size && j + square.x < img.w)
		{
			put_pixel_img(img, j + square.x, i + square.y, square.color);
			j++;
		}
		i++;
	}
}

/*
exit_game - Exit the game, destroy image and window
  Parameter:
    [img] Image instance
*/
int	exit_game(t_win *win)
{
	if (win->win_ptr)
		mlx_destroy_window (win->mlx_ptr, win->win_ptr);
	exit(EXIT_SUCCESS);
}

void * random_food(int tile_size, void * mlx_ptr) {
	static int elements = 101;
	static char * food_list[] = {
		"assets/food/01_dish.xpm","assets/food/02_dish_2.xpm","assets/food/03_dish_pile.xpm","assets/food/05_apple_pie.xpm","assets/food/06_apple_pie_dish.xpm","assets/food/07_bread.xpm","assets/food/08_bread_dish.xpm","assets/food/09_baguette.xpm",
		"assets/food/10_baguette_dish.xpm", "assets/food/11_bun.xpm", "assets/food/12_bun_dish.xpm", "assets/food/13_bacon.xpm", "assets/food/14_bacon_dish.xpm", "assets/food/15_burger.xpm", "assets/food/16_burger_dish.xpm", "assets/food/17_burger_napkin.xpm", "assets/food/18_burrito.xpm", "assets/food/19_burrito_dish.xpm",
		"assets/food/20_bagel.xpm", "assets/food/21_bagel_dish.xpm", "assets/food/22_cheesecake.xpm", "assets/food/23_cheesecake_dish.xpm", "assets/food/24_cheesepuff.xpm", "assets/food/25_cheesepuff_bowl.xpm", "assets/food/26_chocolate.xpm", "assets/food/27_chocolate_dish.xpm", "assets/food/28_cookies.xpm", "assets/food/29_cookies_dish.xpm",
		"assets/food/30_chocolatecake.xpm", "assets/food/31_chocolatecake_dish.xpm", "assets/food/32_curry.xpm", "assets/food/33_curry_dish.xpm", "assets/food/34_donut.xpm", "assets/food/35_donut_dish.xpm", "assets/food/36_dumplings.xpm", "assets/food/37_dumplings_dish.xpm", "assets/food/38_friedegg.xpm", "assets/food/39_friedegg_dish.xpm",
		"assets/food/40_eggsalad.xpm", "assets/food/41_eggsalad_bowl.xpm", "assets/food/42_eggtart.xpm", "assets/food/43_eggtart_dish.xpm", "assets/food/44_frenchfries.xpm", "assets/food/45_frenchfries_dish.xpm", "assets/food/46_fruitcake.xpm", "assets/food/47_fruitcake_dish.xpm", "assets/food/48_garlicbread.xpm", "assets/food/49_garlicbread_dish.xpm",
		"assets/food/50_giantgummybear.xpm", "assets/food/51_giantgummybear_dish.xpm", "assets/food/52_gingerbreadman.xpm", "assets/food/53_gingerbreadman_dish.xpm", "assets/food/54_hotdog.xpm", "assets/food/55_hotdog_sauce.xpm", "assets/food/56_hotdog_dish.xpm", "assets/food/57_icecream.xpm", "assets/food/58_icecream_bowl.xpm", "assets/food/59_jelly.xpm",
		"assets/food/60_jelly_dish.xpm", "assets/food/61_jam.xpm", "assets/food/62_jam_dish.xpm", "assets/food/63_lemonpie.xpm", "assets/food/64_lemonpie_dish.xpm", "assets/food/65_loafbread.xpm", "assets/food/66_loafbread_dish.xpm", "assets/food/67_macncheese.xpm", "assets/food/68_macncheese_dish.xpm", "assets/food/69_meatball.xpm", 
		"assets/food/70_meatball_dish.xpm", "assets/food/71_nacho.xpm", "assets/food/72_nacho_dish.xpm", "assets/food/73_omlet.xpm", "assets/food/74_omlet_dish.xpm", "assets/food/75_pudding.xpm", "assets/food/76_pudding_dish.xpm", "assets/food/77_potatochips.xpm", "assets/food/78_potatochips_bowl.xpm", "assets/food/79_pancakes.xpm",
		"assets/food/80_pancakes_dish.xpm", "assets/food/81_pizza.xpm", "assets/food/82_pizza_dish.xpm", "assets/food/83_popcorn.xpm", "assets/food/84_popcorn_bowl.xpm", "assets/food/85_roastedchicken.xpm", "assets/food/86_roastedchicken_dish.xpm", "assets/food/87_ramen.xpm", "assets/food/88_salmon.xpm", "assets/food/89_salmon_dish.xpm",
		"assets/food/90_strawberrycake.xpm", "assets/food/91_strawberrycake_dish.xpm", "assets/food/92_sandwich.xpm", "assets/food/93_sandwich_dish.xpm", "assets/food/94_spaghetti.xpm", "assets/food/95_steak.xpm", "assets/food/96_steak_dish.xpm", "assets/food/97_sushi.xpm", "assets/food/98_sushi_dish.xpm", "assets/food/99_taco.xpm",
		"assets/food/100_taco_dish.xpm", "assets/food/101_waffle.xpm", "assets/food/102_waffle_dish.xpm"
	};

	return (mlx_xpm_file_to_image(mlx_ptr, food_list[rand() % elements], &tile_size, &tile_size));
}
