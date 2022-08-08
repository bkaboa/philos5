/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utily.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 12:57:28 by czang             #+#    #+#             */
/*   Updated: 2022/08/07 19:53:44 by czang            ###   ########lyon.fr   */
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

void find_time(long long *time)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	*time = (t.tv_sec * 1000) + (t.tv_usec / 1000);
}

long long current_time(void)
{
	long long time;

	find_time(&time);
	return (time);
}

void	upgrade_sleep(long long time, t_philo *philo)
{
	long long	t;
	find_time(&t);
	while (!check_data_stop(philo))
	{
		if (current_time() - t >= time)
			break ;
		usleep(500);
	}
}

int		check_data_stop(t_philo *philos)
{
	pthread_mutex_lock(&philos->data->m_stop);
	if (philos->data->stop)
		return (1);
	pthread_mutex_unlock(&philos->data->m_stop);
	return (0);
}

void	philo_stop(t_philo *philos)
{
	pthread_mutex_lock(&philos->data->m_stop);
		philos->data->stop = 1;
	pthread_mutex_unlock(&philos->data->m_stop);
}

int	ft_atoi(const char *str)
{
	long int	n;

	n = 0;
	if (*str == '-')
		return (-1);
	if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + *str - '0';
		if (n > INT_MAX)
			return (-1);
		str++;
	}
	if (*str != '\0')
		return (-1);
	return ((int)n);
}
