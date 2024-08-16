/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:56 by damin             #+#    #+#             */
/*   Updated: 2024/08/16 14:02:38 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	death_handle(t_philo *philo, long curr_time)
{
	int	ret;

	ret = 0;
	if (sem_wait(philo->print))
		ret = err_return("Error: mutex lock failed");
	if (!ret && printf("%ld %d %s\n", curr_time - \
	(long)philo->start_time, philo->id, "died") == -1)
		ret = err_return("Error: printf failed");
	if (sem_post(philo->print))
		ret = err_return("Error: mutex unlock failed");
	if (ret)
		return (1);
	return (0);
}

void	*death_check(void *ptr)
{
	int		ret;
	t_philo	*philo;
	long	curr_time;

	ret = 0;
	philo = (t_philo *)ptr;
	while (!ret)
	{
		if (sem_wait(philo->die))
			err_exit("Error: sem_wait failed");
		curr_time = get_time();
		if (curr_time - philo->last_eat > philo->time_to_die)
			ret = death_handle(philo, curr_time);
		if (sem_post(philo->die))
			err_exit("Error: sem_post failed");
		if (curr_time == -1)
			exit(1);
		if (!ret && usleep(100))
			err_exit("Error: usleep failed");
	}
	return (NULL);
}

int	child_process(t_philo *philo)
{
	int	ret;

	ret = 0;
	if (pthread_create(&philo->death_checker, NULL, &death_check, &philo))
		err_exit("Error: pthread_create failed");
	while (1)
	{
		if (sem_wait(philo->fork))
			err_exit("Error: sem_wait failed");
		if (print_status(philo, "has taken a fork"))
		{
			sem_post(philo->fork);
			exit(1);
		}
		if (sem_wait(philo->fork))
		{
			sem_post(philo->fork);
			err_exit("Error: sem_wait failed");
		}
		if (print_status(philo, "has taken a fork"))
			ret = 1;
		if (sem_wait(philo->die))
			ret = err_return("Error: sem_wait failed");
		if (print_status(philo, "has taken a fork"))
			ret = 1;
		philo->last_eat = get_time();
		if (philo->last_eat == -1)
			ret = 1;
		else
			philo->meal_cnt++;
		if (sem_post(philo->die))
			ret = err_return("Error: sem_post failed");
		if (!ret && ft_usleep(philo->time_to_eat))
			ret = 1;
		if (sem_post(philo->fork))
			ret = err_return("Erro: sem_post failed");
		if (sem_post(philo->fork))
			err_exit("Erro: sem_post failed");
		return (ret);
		if (philo->meal_cnt == philo->num_of_eat)
			break ;
		if (print_status(philo, "is sleeping"))
			exit(1);
		if (ft_usleep(philo->time_to_sleep))
			exit(1);
		if (print_status(philo, "is thinking"))
			exit(1);
		if (usleep(100))
			exit(1);
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
		if (waitpid(-1, &status, 0) == -1)
			ret = err_return("Error: waitpid failed");
		if (status != 0)
		{
			i = 0;
			while (i < philo->num_of_philo)
			{
				if (kill(pids[i], SIGKILL))
					ret = err_return("Error: kill failed");
				i++;
			}
		}
		i++;
	}
	return (ret);
}

int	start_simulation(t_philo *philo)
{
	int		i;
	pid_t	*pids;
	int		ret;
	int		err_i;

	ret = 0;
	pids = (pid_t *)malloc(philo->num_of_philo * sizeof(pid_t));
	if (!pids)
		return (err_return ("Error: malloc failed"));
	philo->start_time = get_time();
	if (philo->start_time == -1)
		ret = 1;
	i = -1;
	while (!ret && ++i < philo->num_of_philo)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			ret = err_return ("Error: fork failed");
		if (pids[i] == 0)
		{
			philo->id = i;
			break ;
		}
	}
	err_i = i;
	i = 0;
	if (err_i != philo->num_of_philo)
	{
		while (i < err_i)
		{
			if (kill(pids[i], SIGKILL))
				ret = err_return("Error: kill failed");
			i++;
		}
		exit (1);
	}
	if (!ret && pids[philo->id] == 0 && !child_process(philo))
		ret = 1;
	else
		ret = parents_process(philo, pids);
	free(pids);
	return (ret);
}
