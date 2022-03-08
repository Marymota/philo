/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmota <mmota@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 18:32:35 by mmota             #+#    #+#             */
/*   Updated: 2022/03/08 18:36:18 by mmota            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	death(t_sim *sim, t_philos *philo)
{
	long int	death_time;

	pthread_mutex_lock(&sim->time_meal);
	death_time = get_time() - philo->time_meal;
	pthread_mutex_unlock(&sim->time_meal);
	if (death_time > sim->specs->time_to_die)
	{
		pthread_mutex_lock(&sim->write);
		printf("%li %i died\n", death_time, philo->id);
		exit_end(sim);
	}
	pthread_mutex_lock(&sim->eat);
	if (sim->finish_eat >= sim->specs->n_of_philos)
	{
		pthread_mutex_lock(&sim->write);
		printf("All philos are full!\n");
		exit_end(sim);
	}
	pthread_mutex_unlock(&sim->eat);
	return (0);
}

void	free_structs(t_sim *sim)
{
	if (sim->threads != NULL)
		free(sim->threads);
	if (sim->monitor != NULL)
		free(sim->monitor);
	if (sim->philos != NULL)
		free(sim->philos);
	if (sim != NULL)
		free(sim);
}

int	exit_error(t_sim *sim, char *err)
{
	printf("%s\n", err);
	free_structs(sim);
	exit(EXIT_FAILURE);
}

int	exit_end(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->increment);
	sim->end = 1;
	pthread_mutex_unlock(&sim->increment);
	i = -1;
	while (++i < sim->specs->n_of_philos)
		pthread_mutex_destroy(&sim->philos->left_fork);
	pthread_mutex_destroy(&sim->write);
	pthread_mutex_destroy(&sim->increment);
	pthread_mutex_destroy(&sim->time_meal);
	pthread_mutex_destroy(&sim->eat);
	free_structs(sim);
	exit(EXIT_SUCCESS);
}
