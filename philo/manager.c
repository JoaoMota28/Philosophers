/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomanuel <jomanuel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:54:27 by jomanuel          #+#    #+#             */
/*   Updated: 2025/09/03 14:50:42 by jomanuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	check_can_eat(t_data *data)
{
	int			i;
	long long	philo_left;
	long long	philo_right;
	long long	philo_time;

	i = -1;
	while (++i < data->philo_num)
	{
		pthread_mutex_lock(&data->eat_mutex);
		philo_time = data->philosophers[i].last_eat_time;
		if (i == 0)
			philo_left = data->philosophers[data->philo_num - 1].last_eat_time;
		else
			philo_left = data->philosophers[i - 1].last_eat_time;
		if (i == data->philo_num - 1)
			philo_right = data->philosophers[0].last_eat_time;
		else
			philo_right = data->philosophers[i + 1].last_eat_time;
		if (philo_time <= philo_left && philo_time <= philo_right)
			data->philosophers[i].can_eat = true;
		else
			data->philosophers[i].can_eat = false;
		pthread_mutex_unlock(&data->eat_mutex);
	}
}

static int	check_all_eat(t_data *data)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	if (data->loop_number == 0)
		return (0);
	while (++i < data->philo_num)
	{
		pthread_mutex_lock(&data->eat_mutex);
		if (data->philosophers[i].meals_eaten >= data->loop_number)
			count++;
		pthread_mutex_unlock(&data->eat_mutex);
	}
	return (count == data->philo_num);
}

static int	manager_check_death(t_data *data)
{
	int			i;
	long long	time;

	i = -1;
	while (++i < data->philo_num)
	{
		pthread_mutex_lock(&data->eat_mutex);
		time = get_time_ms() - data->philosophers[i].last_eat_time;
		pthread_mutex_unlock(&data->eat_mutex);
		pthread_mutex_lock(&data->death_mutex);
		if (time > data->time_to_die)
		{
			pthread_mutex_unlock(&data->death_mutex);
			pthread_mutex_lock(&data->death_mutex);
			data->philo_over = true;
			pthread_mutex_unlock(&data->death_mutex);
			print_message(&data->philosophers[i], "died");
			return (1);
		}
		else
			pthread_mutex_unlock(&data->death_mutex);
	}
	return (0);
}

void	data_manager(t_data *data)
{
	while (1)
	{
		if (data->philo_num % 2 != 0)
			check_can_eat(data);
		if (check_all_eat(data))
		{
			pthread_mutex_lock(&data->death_mutex);
			data->philo_over = true;
			pthread_mutex_unlock(&data->death_mutex);
			break ;
		}
		if (manager_check_death(data))
			break ;
		usleep(1000);
	}
	terminate_philo(data);
}
