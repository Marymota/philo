#include "philo.h"

void free_structs(t_sim *sim)
{
	if (sim->threads != NULL)
		free(sim->threads);
	if (sim->philos != NULL)
		free(sim->philos);
	if (sim != NULL)
		free(sim);
}

int exit_error(t_sim *sim, char *err)
{
	printf("%s\n", err);
	free_structs(sim);
	exit(EXIT_FAILURE);
}

int exit_end(t_sim *sim)
{
	int i;

	if (sim->specs->n_of_philos > 1)
	{
		i = -1;
		while (++i < sim->specs->n_of_philos)
			pthread_mutex_destroy(sim->philos->right_fork);
	}

	//i = -1;
	//while (++i < sim->specs->n_of_philos)
	//	pthread_exit(&sim->threads[i]);
	free_structs(sim);
	exit(EXIT_SUCCESS);
}
