/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:56 by damin             #+#    #+#             */
/*   Updated: 2024/08/15 13:41:02 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eat(t_philo *philo, int first_fork, int second_fork)
{
	if (pick_fork(philo, first_fork) || pick_fork(philo, second_fork))
		return (1);
	if (pthread_mutex_lock(&philo->data->death_mutex))
		return (err_return("mutex error"));
	if (print_status(philo, "is eating"))
		return (1);
	philo->last_eat = get_time();
	philo->num_of_eat++;
	if (pthread_mutex_unlock(&philo->data->death_mutex))
		return (err_return("mutex error"));
	if (ft_usleep(philo->data->time_to_eat))
		return (1);
	if (drop_fork(philo, first_fork) || drop_fork(philo, second_fork))
		return (1);
	return (0);
}

void	*philo_routine(void *ptr)
{
	t_philo	*philo;
	int		first_fork;
	int		second_fork;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0 && ft_usleep(10))
		return (NULL);
	optimize_fork(philo, &first_fork, &second_fork);
	while (!check_stop_flag(philo->data))
	{
		if (philo_eat(philo, first_fork, second_fork))
			break ;
		if (philo->num_of_eat == philo->data->num_of_eat)
			break ;
		if (print_status(philo, "is sleeping"))
			break ;
		if (ft_usleep(philo->data->time_to_sleep))
			break ;
		if (print_status(philo, "is thinking"))
			break ;
		usleep(100);
	}
	return (NULL);
}

int	mutex_destroy(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
		if (pthread_mutex_destroy(&data->fork_mutex[i++]))
			return (err_return("Error: mutex destroy failed"));
	if (pthread_mutex_destroy(&data->print_mutex) || \
	pthread_mutex_destroy(&data->death_mutex) \
	|| pthread_mutex_destroy(&data->stop_mutex))
		return (err_return("Error: mutex destroy failed"));
	return (0);
}

int	start_simulation(t_data *data, t_philo *philo)
{
	int	i;
	int	ret;

	ret = 0;
	data->th_start = get_time();
	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, &philo_routine, &philo[i]))
			return (err_return("Error: pthread create failed"));
		if (pthread_mutex_lock(&data->death_mutex))
			return (err_return("Error: mutex lock failed"));
		philo[i].last_eat = get_time();
		if (pthread_mutex_unlock(&data->death_mutex))
			return (err_return("Error: mutex unlock failed"));
		i++;
	}
	ret = death_checker(data, philo);
	i = 0;
	while (i < data->num_of_philo)
		if (pthread_join(philo[i++].thread, NULL))
			return (err_return("Error: pthread join failed"));
	if (mutex_destroy(data))
		return (1);
	return (ret);
}
