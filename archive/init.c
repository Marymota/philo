#include "philo.h"
	
/* Initiate the arguments structure based on command line arguments */
void init_arguments(t_arguments *stats, char *argv[])
{
	stats->number_of_philosophers = atoi(argv[1]);
	stats->time_to_die = atoi(argv[2]);
	stats->time_to_eat = atoi(argv[3]);
	stats->time_to_sleep = atoi(argv[4]);
	if (argv[5])
		stats->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	else
		stats->number_of_times_each_philosopher_must_eat = -1;
}