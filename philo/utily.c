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

void	upgrade_sleep(long long time, t_data *data)
{
	long long	t;

	t = find_time();
	while (!data->stop)
	{
		if (find_time() - t >= time)
			break ;
		usleep(500);
	}
}
