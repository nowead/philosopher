/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:32:02 by damin             #+#    #+#             */
/*   Updated: 2024/08/19 22:13:35 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <pthread.h>
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_philo
{
	int			num_of_philo;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_of_eat;
	int			id;
	int			meal_cnt;
	long		start_time;
	long		last_eat;
	sem_t		*fork;
	sem_t		*print;
	sem_t		*die;
	pthread_t	death_checker;
}					t_philo;

// death_checker_bonus.c
void	death_handle(t_philo *philo, long curr_time);
void	*death_check(void *ptr);

// main_bonus.c
int		parse_args(t_philo *philo, int argc, char **argv);
void	clear_philo(t_philo *philo);
int		init_sem(t_philo *philo);
t_philo	*init_philo(int argc, char **argv);

// philo_bonus.c
int		print_status(t_philo *philo, char *status);
void	pick_fork(t_philo *philo, int *ret);
void	drop_fork(t_philo *philo, int *ret);
void	philo_eat(t_philo *philo, int *ret);

// simulation_bonus.c
int		child_process(t_philo *philo);
int		parents_process(t_philo *philo, pid_t *pids);
void	fork_err_handle(t_philo *philo, pid_t *pids, int err_i);
int		repeat_fork(t_philo *philo, pid_t *pids, int i);
int		start_simulation(t_philo *philo);

//philo_utils_bonus.c
int		ft_atoi(const char *str);
long	get_time(void);
int		ft_usleep(long time);
int		err_return(const char *str);
void	err_exit(const char *str);

#endif