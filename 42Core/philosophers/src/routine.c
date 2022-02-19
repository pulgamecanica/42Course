/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 14:28:24 by arosado-          #+#    #+#             */
/*   Updated: 2022/02/15 14:28:25 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	eat(t_philo *p)
{
	p->eat_count++;
	p->last_meal = timestamp_in_ms();
	usleep(p->conf->t_eat);
}

static void	choose_forks(t_philo *p, int *fork1, int *fork2)
{
	int	flag;

	if (p->sit % 2)
	{
		if (p->conf->philo_count % 2)
			flag = 0;
		else
			flag = 1;
	}
	else
	{
		if (p->conf->philo_count % 2)
			flag = 1;
		else
			flag = 0;
	}
	if (flag)
	{
		*fork1 = (p->sit + 1) % (p->conf->philo_count);
		*fork2 = p->sit;
		return ;
	}
	*fork1 = p->sit;
	*fork2 = (p->sit + 1) % (p->conf->philo_count);
}

static void	pick_up_forks(t_philo *p)
{
	int	fork1;
	int	fork2;

	choose_forks(p, &fork1, &fork2);
	pthread_mutex_lock(p->conf->forks + fork1);
	pthread_mutex_lock(&p->conf->m);
	printf("%04lld %02d\e[1;31m has taken fork[%02d]\e[0m\n",
		timestamp_in_ms(), p->sit, fork1);
	pthread_mutex_unlock(&p->conf->m);
	pthread_mutex_lock(p->conf->forks + fork2);
	pthread_mutex_lock(&p->conf->m);
	printf("%04lld %02d\e[1;31m has taken fork[%02d]\e[0m\n",
		timestamp_in_ms(), p->sit, fork2);
	pthread_mutex_unlock(&p->conf->m);
}

static void	let_go_forks(t_philo *p)
{
	pthread_mutex_unlock(p->conf->forks + ((p->sit + 1)
			% p->conf->philo_count));
	pthread_mutex_unlock(p->conf->forks + p->sit);
}

void	*routine(void *arg)
{
	t_philo	*philosopher;

	if (!arg)
		return (NULL);
	philosopher = (t_philo *)arg;
	while (1)
	{
		pick_up_forks(philosopher);
		pthread_mutex_lock(&philosopher->conf->m);
		printf("%04lli %02d\e[1;32m is eating\e[0m\n",
			timestamp_in_ms(), philosopher->sit);
		pthread_mutex_unlock(&philosopher->conf->m);
		eat(philosopher);
		pthread_mutex_lock(&philosopher->conf->m);
		printf("%04lli %02d\e[1;34m is sleeping\e[0m\n",
			timestamp_in_ms(), philosopher->sit);
		pthread_mutex_unlock(&philosopher->conf->m);
		let_go_forks(philosopher);
		usleep(philosopher->conf->t_sleep - 10);
		pthread_mutex_lock(&philosopher->conf->m);
		printf("%04lli %02d\e[1;33m is thinking\e[0m\n",
			timestamp_in_ms(), philosopher->sit);
		pthread_mutex_unlock(&philosopher->conf->m);
	}
	return (NULL);
}
