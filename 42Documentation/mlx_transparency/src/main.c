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
	t_img	image, other_image;

	signal(SIGINT, sig_handler);
	tutorial = new_window(1000, 650, "transparency");
	if (!tutorial.win_ptr)
		return (2);
	image = new_img(300, 300, tutorial);

	{
		image = new_file_img("lotr_map.xpm", tutorial);
		if (!image.img_ptr)
			return (2);
		mlx_put_image_to_window (image.win.mlx_ptr, image.win.win_ptr, image.img_ptr, 0, 0);
	}

	{
		other_image = new_file_img("ring.xpm", tutorial);
		if (!other_image.img_ptr)
			return (2);
		mlx_put_image_to_window (other_image.win.mlx_ptr, other_image.win.win_ptr, other_image.img_ptr, 0, 0);
	}

	mlx_loop(tutorial.mlx_ptr);
	mlx_destroy_window(tutorial.mlx_ptr, tutorial.win_ptr);
	return (0);
}
