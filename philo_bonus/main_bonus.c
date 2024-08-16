/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:49:06 by damin             #+#    #+#             */
/*   Updated: 2024/08/16 13:56:13 by damin            ###   ########.fr       */
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
	if (philo->num_of_philo < 2 || philo->time_to_die < 0 \
	|| philo->time_to_eat < 0 || philo->time_to_sleep < 0 \
	|| (argc == 6 && philo->num_of_eat < 0))
		return (1);
	return (0);
}

void	sem_clear(t_philo *philo)
{
	sem_close(philo->fork);
	sem_close(philo->print);
	sem_close(philo->die);
}

int	init_sem(t_philo *philo)
{
	if (sem_unlink("sem_fork") || sem_unlink("sem_print") || \
	sem_unlink("sem_die"))
		return (err_return("Error: sem_unlink failed"));
	philo->fork = sem_open("sem_fork", O_CREAT, 0644, philo->num_of_philo);
	if (philo->fork == SEM_FAILED)
		return (err_return("Error: sem_open failed"));
	philo->print = sem_open("sem_print", O_CREAT, 0644);
	if (philo->print == SEM_FAILED)
	{
		sem_close(philo->fork);
		return (err_return("Error: sem_open failed"));
	}
	philo->die = sem_open("sem_die", O_CREAT, 0644);
	if (philo->die == SEM_FAILED)
	{
		sem_close(philo->fork);
		sem_close(philo->print);
		return (err_return("Error: sem_open failed"));
	}
	return (0);
}

int	init_philo(t_philo *philo)
{
	philo->id = 0;
	philo->meal_cnt = 0;
	philo->start_time = 0;
	philo->last_eat = 0;
	return (init_sem(philo));
}

int	main(int argc, char **argv)
{
	t_philo	philo;

	if (argc < 5 || argc > 6)
		return (err_return("Error: wrong number of arguments"));
	if (parse_args(&philo, argc, argv))
		return (err_return("Error: wrong arguments"));
	if (init_philo(&philo))
		return (err_return("Error: init_data\n"));
	if (start_simulation(&philo))
	{
		sem_clear(&philo);
		return (err_return("Error: start_simulation"));
	}
	sem_clear(&philo);
	return (0);
}
