/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 12:16:52 by arosado-          #+#    #+#             */
/*   Updated: 2022/02/15 12:16:54 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	create_threads(t_config *config)
{
	int	i;
	int	fail;

	i = 0;
	while (i < config->philo_count)
	{
		fail = pthread_create(config->threads + i, NULL,
				&routine, (config->philosophers) + i);
		if (fail)
			return (0);
		i++;
	}
	return (1);
}

static int	stop_threads(t_config *config)
{
	int	i;
	int	fail;

	i = 0;
	while (i < config->philo_count)
	{
		fail = pthread_detach(config->threads[i]);
		if (fail)
			return (0);
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_config	*config;

	if (ac < 5 || ac > 6)
	{
		printf("Usage: ./philos [# Philosophers] [time_to_die] [time_to_eat] ");
		printf("[time_to_sleep] ([times_each_philosopher_must_eat])\n");
		return (1);
	}
	config = init_config(ac, av);
	if (!config)
		return (1);
	if (!create_threads(config))
		return (2);
	wait_for_completion(config);
	if (!stop_threads(config))
		return (3);
	usleep(10000);
	pthread_mutex_destroy(&config->m);
	free(config->philosophers);
	free(config->forks);
	free(config->threads);
	free(config);
	return (0);
}
