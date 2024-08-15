/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:32:02 by damin             #+#    #+#             */
/*   Updated: 2024/08/15 17:44:38 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	int		num_of_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		num_of_eat;
	int		id;
	int		meal_cnt;
	long	start_time;
	long	last_eat;
	sem_t	fork;
	sem_t	print;
	sem_t	die;
}					t_philo;

// main.c
int		parse_args(t_philo *philo, int argc, char **argv);
void	sem_clear(t_philo *philo);
int		init_sem(t_philo *philo);
int		init_philo(t_philo *philo);

// philo.c


// simulation_bonus.c
int		child_process(t_philo *philo);
int		parents_process(t_philo *philo, pid_t *pid);
int		start_simulation(t_philo *philo);

// death_checker.c


//philo_utils_bonus.c
int		ft_atoi(const char *str);
long	get_time(void);
int		ft_usleep(long time);
int		err_return(const char *str);

#endif