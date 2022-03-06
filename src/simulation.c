/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmota <mmota@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:13:05 by mmota             #+#    #+#             */
/*   Updated: 2022/03/06 22:22:24 by mmota            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int thinking(t_sim *sim, t_philos *philo)
{
	long int	time;

	pthread_mutex_lock(&sim->write);
	time = get_time();
	printf("%li %i is thinking\n", (time - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
	if (sim->end == 0)
	{
		sim->end = 1;
		return (0);
	}
	return (1);
}

void	sleeping(t_sim *sim, t_philos *philo)
{
	long int	time;

	pthread_mutex_lock(&sim->write);
	time = get_time();
	printf("%li %i is sleeping\n", (time - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
	ft_usleep(sim->specs->time_to_sleep);
}

int	eating(t_sim *sim, t_philos *philo)
{
	pthread_mutex_lock(&sim->meal_time);
	philo->time_meal = get_time();
	pthread_mutex_unlock(&sim->meal_time);
	pthread_mutex_lock(&sim->write);
	printf("%li %i is eating\n", (philo->time_meal - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
	pthread_mutex_lock(&sim->meals);
	if (philo->meals_count != -1)
	{
		if (--philo->meals_count == 0)
			sim->finish_eat++;
		if (sim->finish_eat == sim->specs->n_of_philos)
		{
			pthread_mutex_unlock(&sim->meals);
			pthread_mutex_lock(&sim->write);
			printf("All philos are full!\n");
			if (sim->end == 0)
				sim->end = 1;
			return (0);
		}
	}
	pthread_mutex_unlock(&sim->meals);
	ft_usleep(sim->specs->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
	return (1);
}

int	get_forks(t_sim *sim, t_philos *philo)
{
	long int	curr_time;

	curr_time = 0;
	if (pthread_mutex_lock(philo->right_fork) == 0)
	{
		if (pthread_mutex_lock(&philo->left_fork) == 0)
		{
			pthread_mutex_lock(&sim->write);
			curr_time = get_time() - sim->start;
			printf("%li %i has taken a fork\n", curr_time, philo->id);
			printf("%li %i has taken a fork\n", curr_time, philo->id);
			pthread_mutex_unlock(&sim->write);
			return (1);
		}
		else 
			pthread_mutex_unlock(philo->right_fork);
	}
	return (-1);
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
	if (sim->specs->n_of_philos == 1)
	{
		ft_usleep(sim->specs->time_to_die);
		printf("%li %i died\n", get_time() - philo->time_meal, philo->id);
		exit_end(sim);
	}
	else
	{
		while (!sim->end)
		{
			if (!sim->end && get_forks(sim, philo))
			{
				if (!eating(sim, philo) && !sim->end)
					exit_end(sim);
				if (!sim->end)
					sleeping(sim, philo);
				else
					exit_end(sim);
				if (!sim->end)
					thinking(sim, philo);
				else
					exit_end(sim);
			}
			else
				break;
		}
	}
	return (0);
}

void	monitor(t_sim *sim)
{
	static int	i = -1;
	t_philos	*philo;

	if (sim->specs->n_of_philos == 1)
		return;
	pthread_mutex_lock(&sim->increment);
	philo = &sim->philos[++i];
	pthread_mutex_unlock(&sim->increment);
	while ((!death(sim, philo) && !sim->end) && sim->finish_eat != sim->specs->n_of_philos)
		continue ;
	exit_end(sim);
}
