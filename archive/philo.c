#include "philo.h"

void *routine()
{
	sleep(1);
	return (NULL);
}

int main (int argc, char *argv[])
{
	int i;
	/* Check if the number of arguments are valid */
	validate_arguments(argc, argv);
	/* Initiate the arguments structure */
	t_arguments stats;
	pthread_t *philos;
	pthread_mutex_t *forks;
	init_arguments(&stats, argv);
	forks = malloc(sizeof(*forks) * stats.number_of_philosophers);
	philos = malloc(sizeof(*philos) * stats.number_of_philosophers);
	
	i = 0;
	while (i < stats.number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		++i;
	}
	i = 0;
	while (i < stats.number_of_philosophers)
	{
		if (pthread_create(&philos[i], NULL, &routine, NULL) != 0)
		{
			perror("Failed to create thread!");
			exit(EXIT_FAILURE);
		}
		printf("%i Thread started\n", i);
		++i;
	}
	i = 0;
	while (i < stats.number_of_philosophers)
	{
		pthread_join(philos[i], NULL);
		printf("%i Thread finished\n", i);
		++i;
	}
	free(forks);
	free(philos);
	//printf("%d\n%d\n%d\n%d\n%d\n", stats.number_of_philosophers, stats.time_to_die, stats.time_to_eat,stats.time_to_sleep, stats.number_of_times_each_philosopher_must_eat);
	return(0);
}