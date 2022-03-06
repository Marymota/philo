/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmota <mmota@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:12:30 by mmota             #+#    #+#             */
/*   Updated: 2022/03/06 22:15:12 by mmota            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	death(t_sim *sim, t_philos *philo)
{
	long int	death_time;

	pthread_mutex_lock(&sim->meal_time);
	death_time = get_time() - philo->time_meal;
	pthread_mutex_unlock(&sim->meal_time);
	if (death_time > sim->specs->time_to_die && !sim->end)
	{
		pthread_mutex_lock(&sim->write);
		printf("%li %i died\n", death_time, philo->id);
		sim->end = 1;
		return (1);
	}
	return (0);
}

void	free_structs(t_sim *sim)
{	
	if (sim->threads)
		free(sim->threads);
	if (sim->philos)
		free(sim->philos);
	if (sim)
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

	i = -1;
	while (++i < sim->specs->n_of_philos)
		pthread_mutex_destroy(&sim->philos->left_fork);
	pthread_mutex_unlock(&sim->write);
	pthread_mutex_destroy(&sim->write);
	free_structs(sim);
	exit(EXIT_SUCCESS);
}
