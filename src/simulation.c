#include "philo.h"

int death(t_sim *sim, t_philos *philo)
{
	long int death_time;

	if (sim->end)
		exit_end(sim);
	death_time = get_time() - philo->time_last_meal;
	if (death_time > sim->specs->time_to_die)
	{
		sim->end = 1;
		printf("%li %i died\n", death_time, philo->id);
		exit_end(sim);
	}
	return (0);
}

void thinking(t_sim *sim, t_philos *philo)
{
	if (!death(sim, philo))
		printf("%li %i is thinking\n", (get_time() - sim->start), philo->id);
}

void sleeping(t_sim *sim, t_philos *philo)
{
	if (!death(sim, philo))
	{
		printf("%li %i is sleeping\n", (get_time() - sim->start), philo->id);
		ft_usleep(sim->specs->time_to_sleep);
	}
}

void eating(t_sim *sim, t_philos *philo)
{
	philo->time_last_meal = get_time();
	if (!death(sim, philo))
	{
		printf("%li %i is eating\n", (get_time() - sim->start), philo->id);
		ft_usleep(sim->specs->time_to_eat);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->left_fork);
		--philo->meals_count;
	}
}

int get_forks(t_sim *sim, t_philos *philo)
{
	long int curr_time;

	curr_time = 0;
	if (!death(sim, philo))
	{
		if (!death(sim, philo) && pthread_mutex_lock(philo->right_fork) == 0)
		{
			if (!death(sim, philo) && pthread_mutex_lock(&philo->left_fork) == 0)
			{
				curr_time = get_time() - sim->start; 
				printf("%li %i has taken a fork\n", curr_time, philo->id);
				printf("%li %i has taken a fork\n", curr_time, philo->id);
				return (1);
			}
			else
				pthread_mutex_unlock(philo->right_fork);
		}
	}
	return (-1);
}

void *action(void *arg)
{
	static int i = -1;
	t_sim		*sim;
	t_philos		*philo;

	sim = (t_sim *)arg;
	philo = &sim->philos[++i];
	while (!death(sim, philo) && philo->meals_count != 0)
	{
		if (!get_forks(sim, philo))
			break;
		if (!death(sim, philo))
			eating(sim, philo);
		if (!death(sim, philo))
			sleeping(sim, philo);
		if (!death(sim, philo))
			thinking(sim, philo);
	}
	exit_end(sim);
	return 0;
}
