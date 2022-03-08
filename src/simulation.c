/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmota <mmota@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 18:32:17 by mmota             #+#    #+#             */
/*   Updated: 2022/03/08 18:41:04 by mmota            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleeping(t_sim *sim, t_philos *philo)
{
	long int	time;

	time = get_time();
	pthread_mutex_lock(&sim->write);
	printf("%li %i is sleeping\n", (time - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
	ft_usleep(sim->specs->time_to_sleep);
	time = get_time();
	pthread_mutex_lock(&sim->write);
	printf("%li %i is thinking\n", (time - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
}

void	eating(t_sim *sim, t_philos *philo)
{
	pthread_mutex_lock(&sim->time_meal);
	philo->time_meal = get_time();
	pthread_mutex_unlock(&sim->time_meal);
	pthread_mutex_lock(&sim->write);
	printf("%li %i is eating\n", (philo->time_meal - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
	if (philo->meals_count > -1)
	{
		if (--philo->meals_count == 0)
		{
			pthread_mutex_lock(&sim->eat);
			sim->finish_eat++;
			pthread_mutex_unlock(&sim->eat);
		}
	}
	ft_usleep(sim->specs->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

void	get_forks(t_sim *sim, t_philos *philo)
{
	long int	curr_time;

	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->left_fork);
	curr_time = get_time() - sim->start;
	pthread_mutex_lock(&sim->write);
	printf("%li %i has taken a fork\n", curr_time, philo->id);
	printf("%li %i has taken a fork\n", curr_time, philo->id);
	pthread_mutex_unlock(&sim->write);
}

void	*action(void *arg)
{
	static int	i = -1;
	t_sim		*sim;
	t_philos	*philo;

	sim = (t_sim *)arg;
	pthread_mutex_lock(&sim->increment);
	philo = &sim->philos[++i];
	pthread_mutex_unlock(&sim->increment);
	if (sim->specs->n_of_philos == 1 && !sim->end)
	{
		ft_usleep(sim->specs->time_to_die);
		printf("%li %i died\n", get_time() - philo->time_meal, philo->id);
		exit_end(sim);
	}
	else
	{
		while (1)
		{
			get_forks(sim, philo);
			eating(sim, philo);
			sleeping(sim, philo);
		}
	}
	return (0);
}

void	*monitor(void *arg)
{
	static int	i = -1;
	t_sim		*sim;
	t_philos	*philo;

	sim = (t_sim *)arg;
	pthread_mutex_lock(&sim->increment);
	philo = &sim->philos[++i];
	pthread_mutex_unlock(&sim->increment);
	while (!death(sim, philo))
		continue ;
	return (0);
}
