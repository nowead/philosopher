/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:52:53 by damin             #+#    #+#             */
/*   Updated: 2024/08/19 15:54:14 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	death_handle(t_philo *philo, long curr_time)
{
	if (sem_wait(philo->print))
		err_return("Error: sem_wait failed");
	if (printf("%ld %d %s\n", curr_time - \
	(long)philo->start_time \
	, philo->id, "died") == -1)
		err_return("Error: printf failed");
	exit(1);		
}

void	*death_check(void *ptr)
{
	t_philo *philo;
	long	curr_time;
	
	philo = (t_philo *)ptr;
	while (1)
	{
		curr_time = get_time();
		if (sem_wait(philo->die))
			err_exit("Error: sem_wait failed");
		if (curr_time - philo->last_eat > philo->time_to_die)
			death_handle(philo, curr_time);
		if (sem_post(philo->die))
			err_exit("Error: sem_post failed");
		if (curr_time == -1)
			exit(1);
		if (usleep(1000))
			err_exit("Error: usleep failed");
		if (philo->num_of_eat != -1 && philo->meal_cnt == philo->num_of_eat)
			break ;
	}
	return (NULL);
}

