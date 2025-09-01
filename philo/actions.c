/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomanuel <jomanuel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:15:40 by jomanuel          #+#    #+#             */
/*   Updated: 2025/09/01 23:44:30 by jomanuel         ###   ########.fr       */
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
	pthread_mutex_t *first;
    pthread_mutex_t *second;

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
	first  = philo->r_fork;
    second = philo->l_fork;
	if (philo->l_fork < philo->r_fork)
    {
        first  = philo->l_fork;
        second = philo->r_fork;
    }
    pthread_mutex_lock(first);
    pthread_mutex_lock(&data->manager);
	if (!data->philo_over)
	{
		pthread_mutex_unlock(&data->manager);
		print_message(philo, "has taken a fork");
	}
	else
		pthread_mutex_unlock(&data->manager);
    pthread_mutex_lock(second);
    pthread_mutex_lock(&data->manager);
	if (!data->philo_over)
	{
		pthread_mutex_unlock(&data->manager);
		print_message(philo, "has taken a fork");
	}
	else
		pthread_mutex_unlock(&data->manager);
}

static void	philo_unlock_forks(t_philo *philo)
{
	pthread_mutex_t *first;
    pthread_mutex_t *second;

	first  = philo->r_fork;
    second = philo->l_fork;
    if (philo->l_fork < philo->r_fork)
    {
        first  = philo->l_fork;
        second = philo->r_fork;
    }
    pthread_mutex_unlock(second);
    pthread_mutex_unlock(first);
}

void	*philo_routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *) args;
	
	pthread_mutex_lock(&philo->data->manager);
	while (!philo->data->philo_over)
	{
		pthread_mutex_unlock(&philo->data->manager);
		if (philo->data->philo_num == 1)
		{
			print_message(philo, "has taken a fork");
			break ;
		}
		philo_lock_forks(philo->data, philo);
		philo_eat(philo->data, philo);
		philo_unlock_forks(philo);
		philo_sleep(philo->data, philo);
		print_message(philo, "is thinking");
		pthread_mutex_lock(&philo->data->manager);
	}
	pthread_mutex_unlock(&philo->data->manager);
	return (NULL);
}
