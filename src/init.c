#include "philo.h"

t_sim *init_sim(int argc, char *argv[])
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		exit_error(sim, "sim malloc failed");
	sim->start = get_time();
	sim->end = 0;
	sim->specs = init_specs(argc, argv);
	sim->philos = init_philos(sim, sim->start);
	sim->threads = malloc(sizeof(pthread_t) * sim->specs->n_of_philos);
	sim->monitor = malloc(sizeof(pthread_t));
	pthread_mutex_init(&sim->write, NULL);
	if (!sim->threads)
		exit_error(sim, "threads malloc failed");
	init_threads(sim);
	return (sim);
}

//	Init a struct that contains all the info passed on the function call;
t_specs	*init_specs(int argc, char *argv[])
{
	t_specs *specs;

	specs = malloc(sizeof(t_specs));
	specs->n_of_philos = ft_atoi(argv[1]);
	specs->time_to_die = ft_atoi(argv[2]);
	specs->time_to_eat = ft_atoi(argv[3]);
	specs->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		specs->n_times_philos_must_eat = ft_atoi(argv[5]);
	else
		specs->n_times_philos_must_eat = -1;
	return (specs);
}

/*	init_philos() creates a struc that contains all the necessary info
	regarding the life_cycle of each philosopher;
*/
t_philos	*init_philos(t_sim *sim, long int start)
{
	t_philos	*philo;
	int i;

	philo = malloc(sizeof(t_philos) * sim->specs->n_of_philos);
	if (!philo)
	{
		exit_error(sim, "philos malloc failed");
	}
	i = -1;
	while(++i < sim->specs->n_of_philos)
	{
		philo[i].id = i + 1;
		philo[i].meals_count = sim->specs->n_times_philos_must_eat;
		philo[i].time_last_meal = start;
	}
	init_forks(philo, sim->specs);
	return(philo);
}

void	init_forks(t_philos *philo, t_specs *specs)
{
	int i;

	i = -1;
	while (++i < specs->n_of_philos)
	{
		pthread_mutex_init(&philo[i].left_fork, NULL);
		if (specs->n_of_philos == 1)
		{
			philo[0].right_fork = NULL;
			return ;
		}
		else if (i != 0)
			philo[i].right_fork = &philo[i - 1].left_fork;
	}
	philo[0].right_fork = &philo[specs->n_of_philos - 1].left_fork;
}

