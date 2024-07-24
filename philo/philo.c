/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:39:59 by damin             #+#    #+#             */
/*   Updated: 2024/07/24 21:26:29 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((long long)(time.tv_sec * 1000 + time.tv_usec / 1000));
}

int	parse_args(t_data *data, int argc, char **argv)
{
	data->num_of_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->num_of_eat = ft_atoi(argv[5]);
	else
		data->num_of_eat = -1;
	if (data->num_of_philo < 2 || data->time_to_die < 0 \
	|| data->time_to_eat < 0 || data->time_to_sleep < 0 \
	|| (argc == 6 && data->num_of_eat ==data->num_of_eat < 0))
		return (1);
	return (0);
}

int	init_philo(t_data *data, t_philo **philo)
{
	int	i;

	**philo = (t_philo **)malloc(sizeof(t_philo) * data->num_of_philo);
	if (!data->philo)
		return (1);
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->num_of_philo);
	if (!data->forks)
	{
		free(data->philo);
		return (1);
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		philo[i].id = i + 1;
		philo[i].num_of_eat = 0;
		philo[i].data = data;
		philo[i].last_eat = get_time();
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) || pthread_mutex_init(&data->death_mutex, NULL))
	{
		free(data->philo);
		free(data->forks);
		return (1);
	}
	return (0);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	ft_putnbr_fd(get_time() - philo->data->philo[0].last_eat, 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(philo->id, 1);
	ft_putstr_fd(" ", 1);
	ft_putstr_fd(status, 1);
	ft_putstr_fd("\n", 1);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	pthread_create(&philo->death_thread, NULL, &death_check, philo);
	while (1)
	{
		pthread_mutex_lock(&philo->data->forks[philo->id - 1]);
		pthread_mutex_lock(&philo->data->forks[(philo->id) % philo->data->num_of_philo]);
		print_status(philo, "has taken a fork");
		print_status(philo, "has taken a fork");
		print_status(philo, "is eating");
		usleep(philo->data->time_to_eat);
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
		pthread_mutex_unlock(&philo->data->forks[(philo->id) % philo->data->num_of_philo]);
		print_status(philo, "is sleeping");
		usleep(philo->data->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

void	*death_check(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (1)
	{
		if (get_time() - philo->last_eat > philo->data->time_to_die)
		{
			print_status(philo, "died");
			pthread_mutex_lock(&philo->data->death_mutex);
			exit(1);
		}
	}
	return (NULL);

}


int	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			free(data->philo);
			free(data->forks);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_create(&data->philo[i].thread, NULL, &philo_routine, &data->philo[i]))
		{
			free(data->philo);
			free(data->forks);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_join(data->philo[i].thread, NULL))
		{
			free(data->philo);
			free(data->forks);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_mutex_destroy(&data->forks[i]))
		{
			free(data->philo);
			free(data->forks);
			return (1);
		}
		i++;
	}
	free(data->philo);
	free(data->forks);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		err_return("Error: wrong number of arguments\n");
	if (parse_args(&data, argc, argv))
		err_return("Error: wrong arguments\n");
	if (init_data(&data))
		err_return("Error: init_data\n");
	if (start_simulation(&data))
		err_return("Error: start_simulation\n");
	return (0);
}