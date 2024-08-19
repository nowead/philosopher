/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:56 by damin             #+#    #+#             */
/*   Updated: 2024/08/19 15:31:36 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	child_process(t_philo *philo)
{
	int	ret;

	ret = 0;
	philo->last_eat = get_time();
	if (philo->last_eat == -1)
		exit (1);
	if (pthread_create(&philo->death_checker, NULL, &death_check, philo))
		err_exit("Error: pthread_create failed");
	while (1)
	{
		philo_eat(philo, &ret);
		if (philo->num_of_eat != -1 && philo->meal_cnt == philo->num_of_eat)
			break ;
		if (print_status(philo, "is sleeping"))
			exit(1);
		if (ft_usleep(philo->time_to_sleep))
			exit(1);
		if (print_status(philo, "is thinking"))
			exit(1);
		fflush(stdout);// 지워야함
	}
	if (pthread_join(philo->death_checker, NULL))
		err_exit("Error: pthread join failed");
	exit (0);
}

int	parents_process(t_philo *philo, pid_t *pids)
{
	int	status;
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	while (i < philo->num_of_philo)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			i = 0;
			while (i < philo->num_of_philo)
			{
				kill(pids[i], SIGKILL);
				i++;
			}
			break ;
		}
		i++;
	}
	sem_clear(philo);
	return (ret);
}

void	fork_err_handle(t_philo *philo, pid_t *pids, int err_i)
{
	int	i;

	i = 0;
	if (err_i != philo->num_of_philo)
	{
		while (i < err_i)
		{
			if (kill(pids[i], SIGKILL))
				err_return("Error: kill failed");
			i++;
		}
		exit (1);
	}
}

int	repeat_fork(t_philo *philo, pid_t *pids, int i)
{
	pids[i] = fork();
	if (pids[i] == -1)
		return (err_return ("Error: fork failed"));
	if (pids[i] == 0)
	{
		philo->id = i + 1;
		child_process(philo);
	}
	return (0);
}

int	start_simulation(t_philo *philo)
{
	int		i;
	pid_t	*pids;
	int		ret;

	ret = 0;
	pids = (pid_t *)malloc(philo->num_of_philo * sizeof(pid_t));
	if (!pids)
		return (err_return ("Error: malloc failed"));
	memset(pids, 0, philo->num_of_philo * sizeof(pid_t));
	philo->start_time = get_time();
	if (philo->start_time == -1)
		ret = 1;
	i = -1;
	while (!ret && ++i < philo->num_of_philo)
		ret = repeat_fork(philo, pids, i);
	fork_err_handle(philo, pids, i);
	ret = parents_process(philo, pids);
	free(pids);
	return (ret);
}
