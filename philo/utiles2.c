/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 14:20:44 by czang             #+#    #+#             */
/*   Updated: 2022/08/09 14:20:45 by czang            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	philo_have_all_eaten(t_philo *philo)
{
	int		all_eat;

	all_eat = 0;
	pthread_mutex_lock(&philo->data->mutex_eat);
	if (philo->data->num_eat != -1 && \
		philo->num_eat_count >= philo->data->num_eat)
		all_eat = 1;
	pthread_mutex_unlock(&philo->data->mutex_eat);
	return (all_eat);
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
