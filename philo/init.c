/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomanuel <jomanuel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:14:21 by jomanuel          #+#    #+#             */
/*   Updated: 2025/09/02 21:29:26 by jomanuel         ###   ########.fr       */
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
}

static int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	data->forks = malloc(data->philo_num * sizeof(pthread_mutex_t));
	while (++i < data->philo_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
	}
	return (0);
}

static int	init_philos(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = -1;
	data->philosophers = malloc(data->philo_num * sizeof(t_philo));
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
			return (1);
	}
	return (0);
}

int	data_init(int argc, char **argv, t_data *data)
{
	init_vals(argc, argv, data);
	if (pthread_mutex_init(&data->manager, NULL))
		return (1);
	if (init_forks(data) || init_philos(data))
		return (1);
	return (0);
}

void	terminate_philo(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
		pthread_join(data->philosophers[i].thread, NULL);
	i = -1;
	while (++i < data->philo_num)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->manager);
	free(data->forks);
	free(data->philosophers);
}
