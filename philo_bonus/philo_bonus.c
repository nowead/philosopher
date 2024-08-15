/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:39:59 by damin             #+#    #+#             */
/*   Updated: 2024/08/14 19:41:56 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	check_stop_flag(t_data *data)
{
	if (pthread_mutex_lock(&data->stop_mutex))
		return (err_return("mutex error"));
	if (data->stop_simulation)
	{
		if (pthread_mutex_unlock(&data->stop_mutex))
			return (err_return("mutex error"));
		return (2);
	}
	if (pthread_mutex_unlock(&data->stop_mutex))
		return (err_return("mutex error"));
	return (0);
}

int	print_status(t_philo *philo, char *status)
{
	int		stop_flag;
	long	curr_time;

	if (pthread_mutex_lock(&philo->data->print_mutex))
		return (1);
	stop_flag = check_stop_flag(philo->data);
	curr_time = get_time();
	if (curr_time == -1)
		return (1);
	if (stop_flag == 0)
	{
		if (printf("%ld %d %s\n", \
		curr_time - (long)philo->data->th_start, philo->id, status) == -1)
			return (err_return("print error"));
	}
	else if (stop_flag == 1)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (1);
	}
	if (pthread_mutex_unlock(&philo->data->print_mutex))
		return (1);
	return (0);
}

int	pick_fork(t_philo *philo, int fork)
{
	if (pthread_mutex_lock(&philo->data->fork_mutex[fork]))
		return (err_return("mutex error"));
	philo->data->forks[fork] = 1;
	if (print_status(philo, "has taken a fork"))
		return (1);
	return (0);
}

int	drop_fork(t_philo *philo, int fork)
{
	philo->data->forks[fork] = 0;
	if (pthread_mutex_unlock(&philo->data->fork_mutex[fork]))
		return (err_return("mutex error"));
	return (0);
}

void	optimize_fork(t_philo *philo, int *first_fork, int *second_fork)
{
	if (philo->id % 2 == 1)
	{
		*first_fork = philo->id - 1;
		*second_fork = philo->id % philo->data->num_of_philo;
	}
	else if (philo->id % 2 == 0)
	{
		*first_fork = philo->id % philo->data->num_of_philo;
		*second_fork = philo->id - 1;
	}
}
