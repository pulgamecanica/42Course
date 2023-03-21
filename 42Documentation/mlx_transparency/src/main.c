/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname- <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 4242/42/42 42:42:42 by you-              #+#    #+#             */
/*   Updated: 4242/01/42 42:42:42 by you-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "utils.h"
#include "libft.h"
#include <signal.h>

int stop_serv = 0;

void sig_handler() {
	stop_serv = 42;
	ft_putendl_fd("Signal recieved, for now... exit without free\n", 2);
	exit(1);
}

int main(void)
{
 	t_win	tutorial;
	t_img	base_image;
	t_img	bg;
	t_img	ring;

	signal(SIGINT, sig_handler);
	tutorial = new_window(1000, 650, "transparency");
	if (!tutorial.win_ptr)
		return (2);
	base_image = new_img(1000, 650, tutorial);

	{
		bg = new_file_img("lotr_map.xpm", tutorial);
		if (!bg.img_ptr)
			return (2);
		put_img_to_img(base_image, bg, 0, 0);
	}
	{
		ring = new_file_img("ring.xpm", tutorial);
		if (!ring.img_ptr)
			return (2);
		put_img_to_img(base_image, ring, 50, 100);
	}
	mlx_put_image_to_window (base_image.win.mlx_ptr, base_image.win.win_ptr, base_image.img_ptr, 0, 0);
	mlx_loop(tutorial.mlx_ptr);
	mlx_destroy_window(tutorial.mlx_ptr, tutorial.win_ptr);
	return (0);
}
