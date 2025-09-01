/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomanuel <jomanuel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:54:27 by jomanuel          #+#    #+#             */
/*   Updated: 2025/09/01 22:56:56 by jomanuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>

# define PHILO_ERR "Error: Invalid number of arguments.\n"
# define PHILO_USAGE "Usage: ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n"
# define INVALID_ARGS_ERR "Invalid arguments: All arguments must be non-negative and withing int range.\n"
# define PHILO_NUM_ERR "Philosopher number must be non zero.\n"
# define ERROR_INIT "Error initializing philosophers' data.\n"
# define INT_MAX_VAL "2147483648"

typedef struct s_data t_data;


typedef struct	s_philo
{
	int				philo_id;
	t_data			*data;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	long long		last_eat_time;
	int				meals_eaten;
	bool			can_eat;
}	t_philo;

typedef struct s_data
{
	t_philo			*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	manager;
	long long		time_start;
	int				philo_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				loop_number;
	bool			philo_over;
}	t_data;


int	valid_args(int argc, char **argv);
int	data_init(int argc, char **argv, t_data *data);
void	*philo_routine(void *args);
void	data_manager(t_data *data);
void	terminate_philo(t_data *data);

// Utils
int	ft_strcmp(const char *s1, const char *s2);
long long	ft_atoll_abs(const char *nptr);
long long	get_time_ms(void);
void	print_message(t_philo *philo, char *msg);

#endif