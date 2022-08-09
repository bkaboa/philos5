/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 12:57:19 by czang             #+#    #+#             */
/*   Updated: 2022/08/09 01:06:08 by czang            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_data
{
	int				num_philos;
	int				num_forks;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				num_eat;
	int				stop;
	pthread_mutex_t	*mutex_fork;
	pthread_mutex_t	mutex_printf;
	pthread_mutex_t	mutex_time;
	pthread_mutex_t	mutex_stop;
	pthread_mutex_t	mutex_eat;
	long long int	t_start;
	pthread_t		check_monitor;
}t_data;

typedef struct s_philo
{
	int				index;
	int				num_eat_count;
	long long int	t_meal;
	t_data			*data;
	pthread_mutex_t	*lf;
	pthread_mutex_t	*rf;
	pthread_t		pth_t;
}t_philo;

//init
int			init_philos(t_philo **philos, t_data **data, int argc, char **argv);

//unity
int			ft_error(char *str);
long long	find_time(void);
void		upgrade_sleep(long long time, t_philo *philo);
int			ft_atoi(const char *str);
int			check_data_stop(t_philo *philos);
void		philo_stop(t_philo *philos);
int			philo_died(t_philo *philo);
void		philo_have_eaten(t_philo *philo);
int			philo_have_all_eaten(t_philo *philo);
void		get_meal_time(t_philo *philo);
void		destroy_mutex(t_philo *philos);
void destroy_fork(t_philo *philo);

#endif
