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

long long	find_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

int		check_data_stop(t_philo *philos)
{
	int stop;

	stop = 0;
	pthread_mutex_lock(&philos->data->mutex_stop);
	if (philos->data->stop)
		stop = 1;
	pthread_mutex_unlock(&philos->data->mutex_stop);
	return (stop);
}

void philo_stop(t_philo *philos)
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

int	philo_died(t_philo *philo)
{
	int		die;

	die = 0;
	pthread_mutex_lock(&philo->data->mutex_time);
	if (find_time() - philo->t_meal > philo->data->t_die)
		die = 1;
	pthread_mutex_unlock(&philo->data->mutex_time);
	return (die);
}

void	get_meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_time);
	philo->t_meal = find_time();
	pthread_mutex_unlock(&philo->data->mutex_time);
}

void	philo_have_eaten(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_eat);
	if (!check_data_stop(philo))
		philo->num_eat_count += 1;
	pthread_mutex_unlock(&philo->data->mutex_eat);
}

int philo_have_all_eaten(t_philo *philo)
{
	int all_eat;

	all_eat = 0;
	pthread_mutex_lock(&philo->data->mutex_eat);
	if (philo->data->num_eat != -1 && \
		philo->num_eat_count >= philo->data->num_eat)
		all_eat = 1;
	pthread_mutex_unlock(&philo->data->mutex_eat);
	return (all_eat);
}

//void	destroy_mutex(t_philo *philos)
//{
//	int i;
//
//	i = -1;
//	while (++i < philos->data->num_forks)
//		pthread_mutex_destroy(&philos->data->mutex_fork[i]);
//	pthread_mutex_destroy(&philos->data->mutex_eat);
//	pthread_mutex_destroy(&philos->data->mutex_time);
//	pthread_mutex_destroy(&philos->data->mutex_printf);
//	pthread_mutex_destroy(&philos->data->mutex_stop);
//}
