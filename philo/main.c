/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:49:06 by damin             #+#    #+#             */
/*   Updated: 2024/08/13 15:54:17 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_data *data, int argc, char **argv)
{
	data->num_of_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->stop_simulation = 0;
	if (argc == 6)
		data->num_of_eat = ft_atoi(argv[5]);
	else
		data->num_of_eat = -1;
	if (data->num_of_philo < 2 || data->time_to_die < 0 \
	|| data->time_to_eat < 0 || data->time_to_sleep < 0 \
	|| (argc == 6 && data->num_of_eat ==data->num_of_eat < 0))
		return (1);
	data->forks = (int *)malloc(sizeof(int) * data->num_of_philo);
	if (!data->forks)
		return (1);
	data->fork_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->num_of_philo);
	if (!data->fork_mutex)
	{
		free(data->forks);
		return (1);
	}
	return (0);
}

int	init_philo(t_data *data, t_philo **philo)
{
	int	i;

	*philo = malloc(sizeof(t_philo) * data->num_of_philo);
	if (!*philo)
	{
		free(data->forks);
		free(data->fork_mutex);
		return (1);
	}
	i = 0;
	while (i < data->num_of_philo)
		data->forks[i++] = 0;
	i = 0;
	while (i < data->num_of_philo)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].num_of_eat = 0;
		(*philo)[i].data = data;
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) || pthread_mutex_init(&data->death_mutex, NULL)\
	|| pthread_mutex_init(&data->stop_mutex, NULL))
	{
		free_all(*data, *philo);
		return (1);
	}
    i = 0;
	while (i < data->num_of_philo)
		if (pthread_mutex_init(&data->fork_mutex[i++], NULL))
			return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philo;

	if (argc < 5 || argc > 6)
		return (err_return("Error: wrong number of arguments\n"));
	if (parse_args(&data, argc, argv))
		return (err_return("Error: wrong arguments\n"));
	if (init_philo(&data, &philo))
		return (err_return("Error: init_data\n"));
	if (start_simulation(&data, philo))
	{
		free_all(data, philo);
		return (err_return("Error: start_simulation\n"));
	}
	return (0);
}
