/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:08:56 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:10:30 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "fractol.h"
#include <stdio.h>

static void	menu(void)
{
	ft_printf("________                               __");
	ft_printf("                      __\n");
	ft_printf("|        \\                             |");
	ft_printf("  \\                    |  \\\n");
	ft_printf("| $$$$$$$$______   ______    _______  _| ");
	ft_printf("$$_           ______  | \\$\n");
	ft_printf("| $$__   /      \\ |      \\  /       \\|");
	ft_printf("   $$ \\ ______  /      \\ | \\$\n");
	ft_printf("| $$  \\ |  $$$$$$\\ \\$$$$$$\\|  $$$$$$$");
	ft_printf(" \\$$$$$$|      \\|  $$$$$$\\| \\$\n");
	ft_printf("| $$$$$ | $$   \\$$/      $$| $$        |");
	ft_printf(" $$ __\\$$$$$$| $$  | $$| \\$\n");
	ft_printf("| $$    | $$     |  $$$$$$$| $$_____   | ");
	ft_printf("$$|  \\      | $$__/ $$| \\$\n");
	ft_printf("| $$    | $$      \\$$    $$ \\$$     \\ ");
	ft_printf("  \\$$  $$       \\$$    $$| \\$\n");
	ft_printf(" \\$$     \\$$       \\$$$$$$$  \\$$$$$$$");
	ft_printf("    \\$$$$         \\$$$$$$  \\$\n");
	ft_printf("-	M E N U		-\n-%022s -\n", "mandelbrot");
	ft_printf("-%022s -\n", "julia set");
	ft_printf("-%022s -\n", "burning ship");
	ft_printf("Usage: [./fract-ol 'fractal-name' '<options>'] \n");
	ft_printf("Options: '#iterations'\n");
	ft_printf("Default Options: '100'\n");
}

static void	show_plane(t_cplane *p)
{
	int	x;
	int	y;

	x = 0;
	while (x < p->data_ptr->w)
	{
		y = 0;
		while (y < p->data_ptr->h)
		{
			if (get_complex(x, y, p).a == 0)
				put_pixel_img(p->data_ptr, x, y, gen_trgb(0, 50, 20, 255));
			else if (get_complex(x, y, p).b == 0)
				put_pixel_img(p->data_ptr, x, y, gen_trgb(0, 50, 255, 20));
			y++;
		}
		x++;
	}
}

void	*set_function(int ac, char **av)
{
	if (ac >= 2)
	{
		if (ft_strnstr(av[1], "mandelbrot", ft_strlen("mandelbrot")))
			return (create_mandelbrot);
		if (ft_strnstr(av[1], "julia set", ft_strlen("julia set")))
			return (create_julia_set);
		if (ft_strnstr(av[1], "burning ship", ft_strlen("burning ship")))
			return (create_burning_ship);
	}
	menu();
	exit(EXIT_SUCCESS);
}

int	exit_fract_ol(t_window *win)
{
	if (win && win->win_ptr)
		mlx_destroy_window(win->mlx_ptr, win->win_ptr);
	free(win);
	exit(EXIT_SUCCESS);
}

int	main(int ac, char **av)
{
	t_window	*win;
	t_data		*img;
	t_cplane	*p;

	if (ac < 2)
		ft_printf("At least one argument please...\n");
	win = new_window(600, 600, "fract-ol");
	img = new_image(win, 600, 600);
	p = new_cplane(img, ac, av);
	p->fractal_function = set_function(ac, av);
	p->fractal_function(p);
	show_plane(p);
	mlx_key_hook(win->win_ptr, handle_keys, p);
	mlx_mouse_hook(win->win_ptr, handle_mouse, p);
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr, img->img_ptr, 0, 0);
	mlx_hook(win->win_ptr, 17, 0, exit_fract_ol, p);
	mlx_loop(win->mlx_ptr);
	free(img);
	free(p);
	free(win);
	return (0);
}
