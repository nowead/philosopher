/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:32:02 by damin             #+#    #+#             */
/*   Updated: 2024/08/13 18:43:55 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data
{
	int				num_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_of_eat;
	int				*forks;
	int				stop_simulation;
	int				meal_end;
	long			th_start;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	stop_mutex;
	struct s_philo	*philo;
	pthread_mutex_t	*fork_mutex;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				num_of_eat;
	t_data			*data;
	pthread_t		thread;
	long			last_eat;
}					t_philo;


void			*philo_life(void *philo);

//main.c
int				parse_args(t_data *data, int argc, char **argv);
int				init_philo(t_data *data, t_philo **philo);

//philo.c
int				check_stop_flag(t_data *data);
int				print_status(t_philo *philo, char *status);
int				pick_fork(t_philo *philo, int fork);
int				drop_fork(t_philo *philo, int fork);
void			optimize_fork(t_philo *philo, int *first_fork, int *second_fork);

//simulation.c
int				philo_eat(t_philo *philo, int first_fork, int second_fork);
void			*philo_routine(void *ptr);
int				start_simulation(t_data *data, t_philo *philo);
void			death_checker(t_data *data, t_philo *philo);

//philo_utils.c
int				ft_atoi(const char *str);
long			get_time(void);
int				ft_usleep(long time);
int				err_return(const char *str);
void			free_all(t_data data, t_philo *philo);

#endif