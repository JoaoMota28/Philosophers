/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomanuel <jomanuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:14:21 by jomanuel          #+#    #+#             */
/*   Updated: 2025/09/03 17:50:41 by jomanuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_vals(int argc, char **argv, t_data *data)
{
	data->philo_num = ft_atoll_abs(argv[1]);
	data->time_to_die = ft_atoll_abs(argv[2]);
	data->time_to_eat = ft_atoll_abs(argv[3]);
	data->time_to_sleep = ft_atoll_abs(argv[4]);
	data->loop_number = 0;
	if (argc == 6)
		data->loop_number = ft_atoll_abs(argv[5]);
	data->philo_over = false;
	data->time_start = get_time_ms();
	data->forks = NULL;
	data->philosophers = NULL;
}

static int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	data->forks = malloc(data->philo_num * sizeof(pthread_mutex_t));
	if (!data->forks)
	{
		terminate_philo(data, 2, 0);
		return (1);
	}
	while (++i < data->philo_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (terminate_philo(data, 2, i - 1), 1);
	}
	return (0);
}

static int	init_philos(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = -1;
	data->philosophers = malloc(data->philo_num * sizeof(t_philo));
	if (!data->philosophers)
	{
		terminate_philo(data, 1, 0);
		return (1);
	}
	while (++i < data->philo_num)
	{
		philo = &data->philosophers[i];
		philo->data = data;
		philo->meals_eaten = 0;
		philo->philo_id = i + 1;
		philo->last_eat_time = get_time_ms();
		philo->l_fork = &data->forks[i];
		philo->r_fork = &data->forks[(i + 1) % data->philo_num];
		philo->can_eat = true;
		if (pthread_create(&philo->thread, NULL, &philo_routine, philo))
			return (terminate_philo(data, 1, i - 1), 1);
	}
	return (0);
}

int	data_init(int argc, char **argv, t_data *data)
{
	init_vals(argc, argv, data);
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->death_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->eat_mutex, NULL))
		return (1);
	if (init_forks(data) || init_philos(data))
		return (1);
	return (0);
}

void	terminate_philo(t_data *data, int flag, int j)
{
	int	i;

	i = -1;
	if (flag <= 1)
	{
		while (++i < j)
			pthread_join(data->philosophers[i].thread, NULL);
		i = -1;
		while (++i < data->philo_num)
			pthread_mutex_destroy(&data->forks[i]);
	}
	else if (flag == 2)
	{
		while (++i < j)
			pthread_mutex_destroy(&data->forks[i]);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->eat_mutex);
	if (data->forks)
		free(data->forks);
	if (data->philosophers)
		free(data->philosophers);
}
