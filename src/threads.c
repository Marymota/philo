#include "philo.h"

void	init_threads(t_sim *sim)
{
	int i;

	i = -1;
	while(++i < sim->specs->n_of_philos)
	{
		if (pthread_create(&sim->threads[i], NULL, &action, sim) != 0)
			exit_error(sim, "Thread creation failed\n");
	}
	i = -1;
	while(++i < sim->specs->n_of_philos)
	{
		if (pthread_join(sim->threads[i], NULL) != 0)
			exit_error(sim, "Threads join failed\n");
	}
}

