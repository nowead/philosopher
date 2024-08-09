/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:39:59 by damin             #+#    #+#             */
/*   Updated: 2024/08/09 18:11:59 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((long)time.tv_sec * 1000 + (long)time.tv_usec / 1000);
}

void	ft_usleep(long time)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time)
		usleep(100);
}

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
	if (pthread_mutex_init(&data->print_mutex, NULL) || pthread_mutex_init(&data->death_mutex, NULL))
	{
		free_all(data, *philo);
		return (1);
	}
	return (0);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->stop_simulation)
		printf("%ld %d %s\n", get_time() - (long)philo->th_start, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	*philo_routine(void *ptr)
{
	t_philo	*philo;
	int		first_fork;
	int		second_fork;

	philo = (t_philo *)ptr;
	philo->th_start = get_time();
	philo->last_eat = get_time();
	pthread_create(&philo->death_thread, NULL, &death_check, philo);
	if (philo->id % 2 == 0)
		ft_usleep(10);
	if (philo->id % 2 == 1)
	{
		first_fork = philo->id - 1;
		second_fork = philo->id % philo->data->num_of_philo;
	}
	else if (philo->id % 2 == 0)
	{
		first_fork = philo->id % philo->data->num_of_philo;
		second_fork = philo->id - 1;
	}
	while (!philo->data->stop_simulation)
	{
		pthread_mutex_lock(&philo->data->fork_mutex[first_fork]);
		philo->data->forks[first_fork] = 1;
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->fork_mutex[second_fork]);
		philo->data->forks[second_fork] = 1;
		print_status(philo, "has taken a fork");

		pthread_mutex_lock(&philo->data->death_mutex);		
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->data->death_mutex);
		
		if (philo->data->stop_simulation)
			break;
		else
		{
			print_status(philo, "is eating");
			ft_usleep(philo->data->time_to_eat);
			philo->data->forks[first_fork] = 0;
			philo->data->forks[second_fork] = 0;
			pthread_mutex_unlock(&philo->data->fork_mutex[first_fork]);
			pthread_mutex_unlock(&philo->data->fork_mutex[second_fork]);
			print_status(philo, "is sleeping");
			ft_usleep(philo->data->time_to_sleep);
			print_status(philo, "is thinking");
		}
	}
	return (NULL);
}

void	*death_check(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (!philo->data->stop_simulation)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (get_time() - philo->last_eat > philo->data->time_to_die)
		{
			if (!philo->data->stop_simulation)
			{
				print_status(philo, "died");
				philo->data->stop_simulation = 1;
			}
			pthread_mutex_unlock(&philo->data->death_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		usleep(100);
	}
	philo->data->stop_simulation = 1;
	return (NULL);
}


void	free_all(t_data data, t_philo *philo)
{
	free(philo);
	free(data.fork_mutex);
	free(data.forks);
}

int	start_simulation(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
		if (pthread_mutex_init(&data->fork_mutex[i++], NULL))
			return (1);
	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, &philo_routine, &philo[i]))
			return (1);
		i++;
	}
	i = 0;
	while (i < data->num_of_philo)
		if (pthread_join(philo[i++].thread, NULL))
			return (1);
	i = 0;
	while (i < data->num_of_philo)
		if (pthread_mutex_destroy(&data->fork_mutex[i++]))
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
