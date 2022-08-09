/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_mutex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 14:41:05 by czang             #+#    #+#             */
/*   Updated: 2022/08/09 14:41:11 by czang            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutex(t_philo *philos)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&philos->data->mutex_eat);
	pthread_mutex_destroy(&philos->data->mutex_printf);
}
