/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:32:02 by damin             #+#    #+#             */
/*   Updated: 2024/08/12 17:39:48 by damin            ###   ########.fr       */
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
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	stop_mutex;
	struct s_philo	*philo;
	pthread_mutex_t	*fork_mutex;
	pthread_t		death_thread;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				num_of_eat;
	t_data			*data;
	pthread_t		thread;
	long			last_eat;
	long long		th_start;
}					t_philo;


int				parse_args(t_data *data, int argc, char **argv);
int				init_philo(t_data *data, t_philo **philo);
int				start_simulation(t_data *data, t_philo *philo);
void			*philo_life(void *philo);
void			*death_check(void *philo);
void			print_status(t_philo *philo, char *status);

//philo_utils.c
int				ft_atoi(const char *str);
int				err_return(const char *str);
void			free_all(t_data data, t_philo *philo);

#endif