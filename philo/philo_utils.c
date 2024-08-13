/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:37:00 by damin             #+#    #+#             */
/*   Updated: 2024/08/13 17:46:19 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

long	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL))
	{
		printf("gettimeofday error\n");
		return (-1);
	}
	return ((long)time.tv_sec * 1000 + (long)time.tv_usec / 1000);
}

int	ft_usleep(long time)
{
	long	start_time;
	long	present_time;

	start_time = get_time();
	present_time = get_time();
	if (start_time == -1 || present_time == -1)
		return (err_return("gettimeofday error"));
	while (present_time - start_time < time)
	{
		usleep(100);
		present_time = get_time();
		if (present_time == -1)
			return (err_return("gettimeofday error"));
	}
	return (0);
}

int	err_return(const char *str)
{
	printf("%s\n", str);
	return (1);
}

void	free_all(t_data data, t_philo *philo)
{
	free(philo);
	free(data.fork_mutex);
	free(data.forks);
}