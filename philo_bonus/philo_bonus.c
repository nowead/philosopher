/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:39:59 by damin             #+#    #+#             */
/*   Updated: 2024/08/19 21:58:25 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	print_status(t_philo *philo, char *status)
{
	long	curr_time;

	if (sem_wait(philo->print))
		return (err_return("Error: sem_wait failed"));
	curr_time = get_time();
	if (curr_time == -1)
		return (1);
	if (printf("%ld %d %s\n", \
	curr_time - (long)philo->start_time, philo->id, status) == -1)
		return (err_return("print error"));
	if (sem_post(philo->print))
		return (err_return("Error: sem_post failed"));
	return (0);
}

void	pick_fork(t_philo *philo, int *ret)
{
	if (sem_wait(philo->fork))
		err_exit("Error: sem_wait failed");
	if (print_status(philo, "has taken a fork"))
	{
		sem_post(philo->fork);
		exit(1);
	}
	if (sem_wait(philo->fork))
	{
		sem_post(philo->fork);
		err_exit("Error: sem_wait failed");
	}
	if (print_status(philo, "has taken a fork"))
		*ret = 1;
}

void	drop_fork(t_philo *philo, int *ret)
{
	if (sem_post(philo->fork))
		*ret = err_return("Erro: sem_post failed");
	if (sem_post(philo->fork))
		err_exit("Erro: sem_post failed");
}

void	philo_eat(t_philo *philo, int *ret)
{
	pick_fork(philo, ret);
	if (sem_wait(philo->die))
		*ret = err_return("Error: sem_wait failed");
	if (print_status(philo, "is eating"))
		*ret = 1;
	philo->last_eat = get_time();
	if (philo->last_eat == -1)
		*ret = 1;
	if (sem_post(philo->die))
		*ret = err_return("Error: sem_post failed");
	if (!*ret && ft_usleep(philo->time_to_eat))
		*ret = 1;
	philo->meal_cnt++;
	drop_fork(philo, ret);
}
