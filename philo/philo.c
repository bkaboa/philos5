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
	if (!check_data_stop(philo))
		return ;
	pthread_mutex_lock(&philo->data->mutex_printf);
	printf("%lld %d %s\n", \
			current_time() - philo->data->t_start, philo->index, str);
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
		pthread_mutex_lock(&philo->data->time);
		upgrade_sleep(philo->data->t_eat, philo);
		pthread_mutex_unlock(&philo->data->time);
		find_time(&philo->t_meal);
		pthread_mutex_unlock(philo->rf);
		pthread_mutex_unlock(philo->lf);
		if (!check_data_stop(philo))
			philo->num_eat_count += 1;
		philo_print(philo, "is sleeping");
		pthread_mutex_lock(&philo->data->time);
		upgrade_sleep(philo->data->t_sleep, philo);
		pthread_mutex_unlock(&philo->data->time);
	}
	pthread_mutex_unlock(philo->lf);
	pthread_mutex_unlock(philo->rf);
	return (0);
}

void	*check_monitor(void *args)
{
	t_philo	*philos;
	int		i;
	int		flag_all_eat;

	flag_all_eat = 0;
	philos = (t_philo *)args;
	while (!check_data_stop(philos))
	{
		i = -1;
		while (++i < philos->data->num_philos)
		{
			pthread_mutex_lock(&philos->data->time);
			if (current_time() - (philos + i)->t_meal > philos->data->t_die)
			{
				philo_print(philos + i, "died");
				philo_stop(philos);
				break ;
			}
			pthread_mutex_unlock(&philos->data->time);
			if (philos->data->num_eat != -1 && \
				(philos + i)->num_eat_count >= philos->data->num_eat)
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
	find_time(&philos->data->t_start);
	while (++i < philos->data->num_philos)
	{
		find_time(&(philos + i)->t_meal);
		if (pthread_create(&(philos + i)->pth_t, NULL, &start, philos + i))
			ft_error("Error: Failed to create the thread");
		pthread_detach((philos + i)->pth_t);
		usleep(100);
	}
	if (pthread_create(&philos->data->check_monitor, \
						NULL, &check_monitor, philos))
		ft_error("Error: Failed to create the thread");
	if (pthread_join(philos->data->check_monitor, NULL))
		ft_error("Error: Failed to join the thread");
	i = -1;
	while (++i < philos->data->num_forks)
		pthread_mutex_destroy(&philos->data->mutex_fork[i]);
	pthread_mutex_destroy(&philos->data->mutex_printf);
	pthread_mutex_destroy(&philos->data->time);
	pthread_mutex_destroy(&philos->data->m_stop);
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
