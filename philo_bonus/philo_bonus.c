/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:39:59 by damin             #+#    #+#             */
/*   Updated: 2024/08/16 15:48:09 by damin            ###   ########.fr       */
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
