/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_for_completion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 14:24:51 by arosado-          #+#    #+#             */
/*   Updated: 2022/02/15 14:24:52 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static	int	all_satisfied(t_config *conf)
{
	int	i;

	i = 0;
	if (conf->times_to_eat == -1)
		return (0);
	while (i < conf->philo_count)
	{
		if ((conf->philosophers + i)->eat_count < conf->times_to_eat)
			return (0);
		i++;
	}
	return (1);
}

static int	philosopher_died(t_config *conf)
{
	uint64_t	limit;
	uint64_t	timestamp;
	int			i;

	i = 0;
	while (i < conf->philo_count)
	{
		limit = ((conf->philosophers + i)->last_meal + conf->t_die / 1000);
		timestamp = timestamp_in_ms();
		if (limit + 10 < timestamp)
		{
			printf("%04lli %02d\033[0;35m died\033[0m\n", timestamp - 10, i);
			return (1);
		}
		i++;
	}
	return (0);
}

void	wait_for_completion(t_config *conf)
{
	while (1)
	{
		if (all_satisfied(conf))
			return ;
		if (philosopher_died(conf))
			return ;
	}
}
