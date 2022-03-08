/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmota <mmota@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 18:32:35 by mmota             #+#    #+#             */
/*   Updated: 2022/03/08 22:03:37 by mmota            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	death(t_sim *sim, t_philos *philo)
{
	pthread_mutex_lock(&sim->time_meal);
	//printf("id: %d\n", philo->id);
	if (get_time() - philo->time_meal > sim->specs->time_to_die)
	{
		pthread_mutex_lock(&sim->write);
		printf("%li %i died\n", get_time() - sim->start, philo->id);
		return(1);
	}
	pthread_mutex_lock(&sim->eat);
	if (sim->finish_eat >= sim->specs->n_of_philos)
	{
		pthread_mutex_lock(&sim->write);
		printf("All philos are full!\n");
		return(1);
	}
	pthread_mutex_unlock(&sim->eat);
	pthread_mutex_unlock(&sim->time_meal);
	return (0);
}

void	free_structs(t_sim *sim)
{
	int i = 0;
	if (sim->threads != NULL)
		free(sim->threads);
	if (sim->monitor != NULL)
		free(sim->monitor);
	i = -1;
	while (++i < sim->specs->n_of_philos)
		pthread_mutex_destroy(&sim->philos->left_fork);
	if (sim->philos != NULL)
		free(sim->philos);
	pthread_mutex_destroy(&sim->write);
	pthread_mutex_destroy(&sim->increment);
	pthread_mutex_destroy(&sim->time_meal);
	pthread_mutex_destroy(&sim->eat);
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
	pthread_mutex_lock(&sim->increment);
	sim->end = 1;
	pthread_mutex_unlock(&sim->increment);
	exit(EXIT_SUCCESS);
}
