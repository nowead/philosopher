/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:49:06 by damin             #+#    #+#             */
/*   Updated: 2024/08/20 14:41:41 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	parse_args(t_philo *philo, int argc, char **argv)
{
	philo->num_of_philo = ft_atoi(argv[1]);
	philo->time_to_die = ft_atoi(argv[2]);
	philo->time_to_eat = ft_atoi(argv[3]);
	philo->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		philo->num_of_eat = ft_atoi(argv[5]);
	else
		philo->num_of_eat = -1;
	if (philo->num_of_philo < 1 || philo->time_to_die < 0 \
	|| philo->time_to_eat < 0 || philo->time_to_sleep < 0 \
	|| (argc == 6 && philo->num_of_eat < 0))
		return (1);
	return (0);
}

void	clear_philo(t_philo *philo)
{
	sem_close(philo->fork);
	sem_close(philo->print);
	sem_close(philo->die);
	sem_unlink("/sem_fork");
	sem_unlink("/sem_print");
	sem_unlink("/sem_die");
	free(philo);
}

int	init_sem(t_philo *philo)
{
	sem_unlink("/sem_fork");
	sem_unlink("/sem_print");
	sem_unlink("/sem_die");
	philo->fork = sem_open("/sem_fork", O_CREAT, 0644, philo->num_of_philo);
	if (philo->fork == SEM_FAILED)
		return (err_return("Error: sem_open failed"));
	philo->print = sem_open("/sem_print", O_CREAT, 0644, 1);
	if (philo->print == SEM_FAILED)
	{
		sem_close(philo->fork);
		return (err_return("Error: sem_open failed"));
	}
	philo->die = sem_open("/sem_die", O_CREAT, 0644, 1);
	if (philo->die == SEM_FAILED)
	{
		sem_close(philo->fork);
		sem_close(philo->print);
		return (err_return("Error: sem_open failed"));
	}
	return (0);
}

t_philo	*init_philo(int argc, char **argv)
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = 0;
	philo->meal_cnt = 0;
	philo->start_time = 0;
	philo->last_eat = 0;
	if (parse_args(philo, argc, argv))
	{
		free(philo);
		err_return("Error: wrong arguments");
		return (NULL);
	}
	if (init_sem(philo))
	{
		free(philo);
		return (NULL);
	}
	return (philo);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;

	philo = NULL;
	if (argc < 5 || argc > 6)
		return (err_return("Error: wrong number of arguments"));
	philo = init_philo(argc, argv);
	if (!philo)
		return (err_return("Error: init_philo"));
	if (start_simulation(philo))
	{
		clear_philo(philo);
		return (err_return("Error: start_simulation"));
	}
	clear_philo(philo);
	return (0);
}
