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
	if (!philo->data->stop)
		printf("%lld %d %s\n", \
			find_time() - philo->data->t_start, philo->index, str);
	pthread_mutex_unlock(&philo->data->mutex_printf);
}

void	*start(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (!philo->data->stop)
	{
		philo_print(philo, "is thinking");
		pthread_mutex_lock(philo->lf);
		philo_print(philo, "has taken a fork");
		pthread_mutex_lock(philo->rf);
		philo_print(philo, "has taken a fork");
		philo_print(philo, "is eating");
		upgrade_sleep(philo->data->t_eat, philo->data);
		philo->t_meal = find_time();
		pthread_mutex_unlock(philo->rf);
		pthread_mutex_unlock(philo->lf);
		pthread_mutex_lock(&philo->data->m_stop);
		if (!philo->data->stop)
			philo->num_eat_count += 1;
		pthread_mutex_unlock(&philo->data->m_stop);
		philo_print(philo, "is sleeping");
		//pthread_mutex_lock(&philo->data->time);
		upgrade_sleep(philo->data->t_sleep, philo->data);
		//thread_mutex_unlock(&philo->data->time);
	}
	return (0);
}

void	*check_monitor(void *args)
{
	t_philo	*philos;
	int		i;
	int		stop;
	int		flag_all_eat;

	stop = 0;
	flag_all_eat = 0;
	philos = (t_philo *)args;
	while (!stop)
	{
		i = -1;
		while (++i < philos->data->num_philos)
		{
			if (find_time() - (philos + i)->t_meal > philos->data->t_die)
			{
				philo_print(philos + i, "died");
				stop = 1;
				break ;
			}
			if (philos->data->num_eat != -1 && \
				(philos + i)->num_eat_count >= philos->data->num_eat)
				flag_all_eat++;
		}
		if (flag_all_eat == philos->data->num_philos)
			break ;
	}
	pthread_mutex_lock(&philos->data->m_stop);
	philos->data->stop = 1;
	pthread_mutex_unlock(&philos->data->m_stop);
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
		usleep(50);
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
