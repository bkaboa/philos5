/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utily.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 12:57:28 by czang             #+#    #+#             */
/*   Updated: 2022/08/09 14:41:13 by czang            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *str)
{
	int	i;

	i = 0;
	while (*(str + i))
		i++;
	write(2, str, i);
	write(2, "\n", 1);
	return (-1);
}

long long	find_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

int	check_data_stop(t_philo *philos)
{
	int		stop;

	stop = 0;
	pthread_mutex_lock(&philos->data->mutex_stop);
	if (philos->data->stop)
		stop = 1;
	pthread_mutex_unlock(&philos->data->mutex_stop);
	return (stop);
}

void	philo_stop(t_philo *philos)
{
	pthread_mutex_lock(&philos->data->mutex_stop);
	philos->data->stop = 1;
	pthread_mutex_unlock(&philos->data->mutex_stop);
}

void	upgrade_sleep(long long time, t_philo *philo)
{
	long long	t;

	t = find_time();
	while (!check_data_stop(philo))
	{
		if (find_time() - t >= time)
			break ;
		usleep(500);
	}
}
