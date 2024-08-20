/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:22:08 by damin             #+#    #+#             */
/*   Updated: 2024/08/20 11:01:59 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	death_handle(t_data *data, t_philo *philo, long curr_time, int i)
{
	int	ret;

	ret = 0;
	if (!ret && pthread_mutex_lock(&data->stop_mutex))
			ret = err_return("Error: mutex lock failed");
	if (!ret)
		philo->data->stop_simulation = 1;
	if (!ret && pthread_mutex_unlock(&data->stop_mutex))
		ret = err_return("Error: mutex unlock failed");
	if (pthread_mutex_lock(&philo->data->print_mutex))
		ret = err_return("Error: mutex lock failed");
	if (!ret && printf("%ld %d %s\n", curr_time - \
	(long)philo[i].data->th_start \
	, philo[i].id, "died") == -1)
		ret = err_return("Error: printf failed");
	if (pthread_mutex_unlock(&philo->data->print_mutex))
		ret = err_return("Error: mutex unlock failed");
	if (ret)
		return (1);
	return (0);
}

int	death_process(t_data *data, t_philo *philo, int i)
{
	int		ret;
	long	curr_time;

	ret = 0;
	if (pthread_mutex_lock(&data->death_mutex))
		ret = err_return("Error: mutex lock failed");
	curr_time = get_time();
	if (!ret && (curr_time - philo[i].last_eat > data->time_to_die))
		ret = death_handle(data, philo, curr_time, i);
	if (pthread_mutex_unlock(&data->death_mutex))
		ret = err_return("Error: mutex unlock failed");
	if (curr_time == -1)
		ret = 1;
	if (!ret && usleep(100))
		ret = err_return("Error: usleep failed");
	if (ret)
		return (1);
	return (0);
}

int	death_checker(t_data *data, t_philo	*philo)
{
	int		i;
	int		ret;

	ret = 0;
	while (!data->meal_end)
	{
		i = -1;
		while (!ret && ++i < data->num_of_philo && !data->stop_simulation)
			ret = death_process(data, philo, i);
		if (ret)
		{
			pthread_mutex_lock(&data->stop_mutex);
				philo->data->stop_simulation = 1;
			pthread_mutex_unlock(&data->stop_mutex);
		}
		if (data->stop_simulation)
			break ;
		i = 0;
		while (data->num_of_eat != -1 && \
		i < data->num_of_philo && philo[i].num_of_eat >= data->num_of_eat)
			i++;
		if (i == data->num_of_philo)
			data->meal_end = 1;
	}
	return (ret);
}
