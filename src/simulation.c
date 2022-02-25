#include "philo.h"

void	sleeping(t_sim *sim, t_philos *philo)
{
	long int	time;

	time = get_time();
	if (pthread_mutex_lock(&sim->write) == 0)
		printf("%li %i is sleeping\n", (time - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
	ft_usleep(sim->specs->time_to_sleep);
	if (pthread_mutex_lock(&sim->write) == 0)
		printf("%li %i is thinking\n", (time - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
}

void	eating(t_sim *sim, t_philos *philo)
{
	philo->time_last_meal = get_time();
	if (pthread_mutex_lock(&sim->write) == 0)
		printf("%li %i is eating\n", (philo->time_last_meal - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
	ft_usleep(sim->specs->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

int	get_forks(t_sim *sim, t_philos *philo)
{
	long int	curr_time;

	curr_time = 0;
	if (pthread_mutex_lock(philo->right_fork) == 0)
	{
		if (pthread_mutex_lock(&philo->left_fork) == 0)
		{
			curr_time = get_time() - sim->start;
			if (pthread_mutex_lock(&sim->write) == 0)
				printf("%li %i has taken a fork\n", curr_time, philo->id);
			pthread_mutex_unlock(&sim->write);
			if (pthread_mutex_lock(&sim->write) == 0)
				printf("%li %i has taken a fork\n", curr_time, philo->id);
			pthread_mutex_unlock(&sim->write);
		}
	}
	return (-1);
}

void	*action(void *arg)
{
	static int	i = -1;
	t_sim		*sim;
	t_philos	*philo;

	sim = (t_sim *)arg;
	philo = &sim->philos[++i];
	if (sim->specs->n_of_philos == 1)
	{
		ft_usleep(sim->specs->time_to_die);
		printf("%li %i died\n", get_time() - philo->time_last_meal, philo->id);
		exit_end(sim);
	}
	else
	{
		while (!death(sim, philo) && philo->meals_count != 0)
		{
			if (!get_forks(sim, philo))
				break ;
			eating(sim, philo);
			sleeping(sim, philo);
		}
	}
	exit_end(sim);
	return (0);
}

void	*monitor(void *arg)
{
	static int	i = -1;
	t_sim		*sim;
	t_philos	*philo;

	sim = (t_sim *)arg;
	philo = &sim->philos[++i];
	while (!death(sim, philo) && philo->meals_count != 0)
		continue ;
	exit_end(sim);
	return (0);
}
