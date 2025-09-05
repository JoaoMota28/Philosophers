/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomanuel <jomanuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:15:40 by jomanuel          #+#    #+#             */
/*   Updated: 2025/09/03 21:23:04 by jomanuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_sleep(t_data *data, t_philo *philo)
{
	long long	start;
	bool		over;

	start = get_time_ms();
	print_message(philo, "is sleeping");
	while (get_time_ms() - start < data->time_to_sleep)
	{
		usleep(500);
		pthread_mutex_lock(&data->death_mutex);
		over = data->philo_over;
		pthread_mutex_unlock(&data->death_mutex);
		if (over)
			break ;
	}
}

void	philo_eat(t_data *data, t_philo *philo)
{
	long long	start;
	bool		over;

	start = get_time_ms();
	print_message(philo, "is eating");
	while (get_time_ms() - start < data->time_to_eat)
	{
		usleep(500);
		pthread_mutex_lock(&data->death_mutex);
		over = data->philo_over;
		pthread_mutex_unlock(&data->death_mutex);
		if (over)
			break ;
	}
	pthread_mutex_lock(&data->eat_mutex);
	philo->last_eat_time = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&data->eat_mutex);
}

static void	philo_lock_forks(t_data *data, t_philo *philo)
{
	can_eat_loop(data, philo);
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(philo->l_fork);
		print_message(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_fork);
		print_message(philo, "has taken a fork");
	}
}

static void	philo_unlock_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}

void	*philo_routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *) args;
	if (philo->data->philo_num == 1)
	{
		print_message(philo, "has taken a fork");
		return (NULL);
	}
	pthread_mutex_lock(&philo->data->death_mutex);
	while (!philo->data->philo_over)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		if (philo->philo_id % 2 == 0)
			usleep(1000);
		philo_lock_forks(philo->data, philo);
		philo_eat(philo->data, philo);
		philo_unlock_forks(philo);
		philo_sleep(philo->data, philo);
		print_message(philo, "is thinking");
		pthread_mutex_lock(&philo->data->death_mutex);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (NULL);
}
