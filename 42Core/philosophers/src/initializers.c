/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 12:15:10 by arosado-          #+#    #+#             */
/*   Updated: 2022/02/15 12:15:11 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_philo	init_philo(int sit, t_config *conf)
{
	t_philo	p;

	p.conf = conf;
	p.sit = sit;
	p.eat_count = 0;
	p.last_meal = 0;
	return (p);
}

static int	init_threads_and_mutexs(t_config *c)
{
	int	i;

	i = 0;
	c->threads = (pthread_t *)malloc(sizeof(pthread_t) * c->philo_count);
	c->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* c->philo_count);
	if (!c->threads || !c->forks)
		return (0);
	while (i < c->philo_count)
	{
		c->philosophers[i] = init_philo(i, c);
		pthread_mutex_init(c->forks + i, NULL);
		i++;
	}
	return (1);
}

t_config	*init_config(int ac, char **av)
{
	t_config	*config;

	config = (t_config *)malloc(sizeof(t_config));
	if (!config || ft_atoi(av[1]) < 1)
		return (NULL);
	config->philo_count = ft_atoi(av[1]);
	config->philosophers = (t_philo *)malloc(sizeof(t_philo)
			* config->philo_count);
	if (!config->philosophers)
		return (NULL);
	config->t_eat = ft_atoi(av[3]) * 1000;
	config->t_die = ft_atoi(av[2]) * 1000;
	config->t_sleep = ft_atoi(av[4]) * 1000;
	if (ac == 5)
		config->times_to_eat = -1;
	else
		config->times_to_eat = ft_atoi(av[5]);
	if (!init_threads_and_mutexs(config))
		return (NULL);
	pthread_mutex_init(&config->m, NULL);
	return (config);
}
