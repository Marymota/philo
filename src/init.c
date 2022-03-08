
#include "philo.h"

t_sim	*init_sim(int argc, char *argv[])
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		exit_error(sim, "sim malloc failed");
	sim->start = get_time();
	sim->finish_eat = 0;
	sim->end = 0;
	sim->specs = init_specs(argc, argv);
	sim->philos = init_philos(sim, sim->start);
	sim->threads = malloc(sizeof(pthread_t) * sim->specs->n_of_philos);
	if (!sim->threads)
		exit_error(sim, "sim malloc failed");
	sim->monitor = malloc(sizeof(pthread_t));
	if (!sim->monitor)
		exit_error(sim, "sim malloc failed");
	pthread_mutex_init(&sim->write, NULL);
	pthread_mutex_init(&sim->increment, NULL);
	pthread_mutex_init(&sim->time_meal, NULL);
	pthread_mutex_init(&sim->eat, NULL);
	pthread_mutex_init(&sim->death, NULL);
	if (!sim->threads)
		exit_error(sim, "threads malloc failed");
	init_threads(sim);
	return (sim);
}

t_specs	*init_specs(int argc, char *argv[])
{
	t_specs	*specs;

	specs = malloc(sizeof(t_specs));
	if (!specs)
		exit(EXIT_FAILURE);
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

t_philos	*init_philos(t_sim *sim, long int start)
{
	t_philos	*philo;
	int			i;

	philo = malloc(sizeof(t_philos) * sim->specs->n_of_philos);
	if (!philo)
		exit_error(sim, "philos malloc failed");
	i = -1;
	while (++i < sim->specs->n_of_philos)
	{
		philo[i].id = i + 1;
		philo[i].meals_count = sim->specs->n_times_philos_must_eat;
		philo[i].time_last_meal = start;
	}
	init_forks(philo, sim->specs);
	return (philo);
}

void	init_forks(t_philos *philo, t_specs *specs)
{
	int	i;

	i = -1;
	while (++i < specs->n_of_philos)
	{
		if (pthread_mutex_init(&philo[i].left_fork, NULL) == 0)
		{
			if (specs->n_of_philos == 1)
			{
				philo[0].right_fork = NULL;
				return ;
			}
			else if (i != 0)
				philo[i].right_fork = &philo[i - 1].left_fork;
		}
		else
			exit(EXIT_FAILURE);
	}
	philo[0].right_fork = &philo[specs->n_of_philos - 1].left_fork;
}

void	init_threads(t_sim *sim)
{
	int	i;

	if (sim->specs->n_of_philos != 1)
	{
		if (pthread_create(sim->monitor, NULL, &monitor, sim) != 0)
			exit_error(sim, "Thread creation failed\n");
	}
	i = -1;
	while (++i < sim->specs->n_of_philos && !sim->end)
	{
		if (pthread_create(&sim->threads[i], NULL, &action, sim) != 0)
			exit_error(sim, "Thread creation failed\n");
	}
	i = -1;
	while (++i < sim->specs->n_of_philos)
	{
		if (pthread_join(sim->threads[i], NULL) != 0)
			exit_error(sim, "Threads join failed\n");
	}
}