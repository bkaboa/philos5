/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 12:57:00 by czang             #+#    #+#             */
/*   Updated: 2022/08/07 20:01:40 by czang            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->mutex_printf);
	if (!check_data_stop(philo))
		printf("%lld %d %s\n", \
			find_time() - philo->data->t_start, philo->index, str);
	pthread_mutex_unlock(&philo->data->mutex_printf);
}

void	*start(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (!check_data_stop(philo))
	{
		philo_print(philo, "is thinking");
		pthread_mutex_lock(philo->rf);
		philo_print(philo, "has taken a fork");
		pthread_mutex_lock(philo->lf);
		philo_print(philo, "has taken a fork");
		philo_print(philo, "is eating");
		upgrade_sleep(philo->data->t_eat, philo);
		pthread_mutex_unlock(philo->rf);
		pthread_mutex_unlock(philo->lf);
		get_meal_time(philo);
		philo_have_eaten(philo);
		philo_print(philo, "is sleeping");
		upgrade_sleep(philo->data->t_sleep, philo);
	}
	return (0);
}

void	*check_monitor(void *args)
{
	t_philo	*philos;
	int		i;
	int		flag_all_eat;

	philos = (t_philo *)args;
	while (!check_data_stop(philos))
	{
		flag_all_eat = 0;
		i = -1;
		while (++i < philos->data->num_philos)
		{
			if (philo_died(philos + i))
			{
				philo_print(philos + i, "died");
				philo_stop(philos);
				break ;
			}
			if (philo_have_all_eaten(philos + i))
				flag_all_eat++;
		}
		if (flag_all_eat == philos->data->num_philos)
			philo_stop(philos);
	}
	return (0);
}

void	philo_start(t_philo *philos)
{
	int	i;

	i = -1;
	philos->data->t_start = find_time();
	while (++i < philos->data->num_philos)
	{
		(philos + i)->t_meal = find_time();
		if (pthread_create(&(philos + i)->pth_t, NULL, &start, philos + i))
			ft_error("Error: Failed to create the thread");
		pthread_detach((philos + i)->pth_t);
		usleep(300);
	}
	if (pthread_create(&philos->data->check_monitor, \
						NULL, &check_monitor, philos))
		ft_error("Error: Failed to create the thread");
	if (pthread_join(philos->data->check_monitor, NULL))
		ft_error("Error: Failed to join the thread");
	//destroy_mutex(philos);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;

	if (argc < 5 || argc > 6)
		return (ft_error("Error: Wrong number of arguments"));
	if (init_philos(&philos, argc, argv) == -1)
	{
		if (philos->data)
			free(philos->data);
		if (philos)
			free(philos);
		return (0);
	}
	philo_start(philos);
	return (0);
}
