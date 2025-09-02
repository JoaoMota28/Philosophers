/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomanuel <jomanuel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:15:40 by jomanuel          #+#    #+#             */
/*   Updated: 2025/09/02 23:14:25 by jomanuel         ###   ########.fr       */
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
		pthread_mutex_lock(&data->manager);
		over = data->philo_over;
		pthread_mutex_unlock(&data->manager);
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
		pthread_mutex_lock(&data->manager);
		over = data->philo_over;
		pthread_mutex_unlock(&data->manager);
		if (over)
			break ;
	}
	pthread_mutex_lock(&data->manager);
	philo->last_eat_time = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&data->manager);
}

static void	philo_lock_forks(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->manager);
	while (!philo->can_eat)
	{
		pthread_mutex_unlock(&data->manager);
		usleep(100);
		pthread_mutex_lock(&data->manager);
		if (data->philo_over)
			break ;
	}
	pthread_mutex_unlock(&data->manager);
	pthread_mutex_lock(philo->r_fork);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(philo->l_fork);
	print_message(philo, "has taken a fork");
}

/*static void	philo_pause(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		if (philo->data->philo_num <=10)
			usleep(100);
		else if (philo->data->philo_num <=50)
			usleep(500);
		else if (philo->data->philo_num <=100)
			usleep(1000);
		else
			usleep(3000);
	}
}*/

void	*philo_routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *) args;
	if (philo->data->philo_num == 1)
	{
		print_message(philo, "has taken a fork");
		return (NULL);
	}
	//philo_pause(philo);
	if (philo->philo_id % 2 == 0)
		usleep(1000);
	pthread_mutex_lock(&philo->data->manager);
	while (!philo->data->philo_over)
	{
		pthread_mutex_unlock(&philo->data->manager);
		philo_lock_forks(philo->data, philo);
		philo_eat(philo->data, philo);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		philo_sleep(philo->data, philo);
		print_message(philo, "is thinking");
		pthread_mutex_lock(&philo->data->manager);
	}
	pthread_mutex_unlock(&philo->data->manager);
	return (NULL);
}
