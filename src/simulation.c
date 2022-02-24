#include "philo.h"

int check_death(t_sim *sim, t_philos *philo)
{
	long int death_time;

	death_time = get_time() - philo->time_last_meal;
	if (death_time > sim->specs->time_to_die)
	{
		sim->end = 1;
		printf("%li %i died\n", death_time, philo->id);
		pthread_mutex_lock(&sim->write);
		exit_end(sim);
	}
	return (0);	
}

int death(t_sim *sim, t_philos *philo)
{
	long int death_time;

	death_time = get_time() - philo->time_last_meal;
	if (death_time >= sim->specs->time_to_die)
	{
		sim->end = 1;
		printf("%li %i died\n", death_time, philo->id);
		pthread_mutex_lock(&sim->write);
		exit_end(sim);
	}
	return (0);
}

void thinking(t_sim *sim, t_philos *philo)
{
	if (pthread_mutex_lock(&sim->write) == 0 && !death(sim, philo))
		printf("%li %i is thinking\n", (get_time() - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
}

void sleeping(t_sim *sim, t_philos *philo)
{
	if (!death(sim, philo) && pthread_mutex_lock(&sim->write) == 0)
		printf("%li %i is sleeping\n", (get_time() - sim->start), philo->id);
	pthread_mutex_unlock(&sim->write);
	if (!sim->end && !death(sim, philo))
		ft_usleep(sim->specs->time_to_sleep, sim);
}

void eating(t_sim *sim, t_philos *philo)
{
	if (!death(sim, philo) && ! sim->end)
	{
		philo->time_last_meal = get_time();
		if (pthread_mutex_lock(&sim->write) == 0 && !sim->end)
			printf("%li %i is eating\n", (philo->time_last_meal - sim->start), philo->id);
		pthread_mutex_unlock(&sim->write);	
	}
	if (!sim->end && !death(sim, philo))
	{
		ft_usleep(sim->specs->time_to_eat, sim);
		philo->meals_count--;	
	}
	if (!sim->end && !death(sim, philo))
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->left_fork);
	}
}

int get_forks(t_sim *sim, t_philos *philo)
{
	long int curr_time;

	curr_time = 0;
	if (!death(sim, philo))
	{
		if (pthread_mutex_lock(philo->right_fork) == 0 && pthread_mutex_lock(&philo->left_fork) == 0)
		{
			curr_time = get_time() - sim->start;
			if (pthread_mutex_lock(&sim->write) == 0 && !sim->end)
				printf("%li %i has taken a fork\n", curr_time, philo->id);
			pthread_mutex_unlock(&sim->write);
			if (pthread_mutex_lock(&sim->write) == 0 && !sim->end)
				printf("%li %i has taken a fork\n", curr_time, philo->id);
			pthread_mutex_unlock(&sim->write);
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
	if (sim->specs->n_of_philos == 1)
	{
		ft_usleep(sim->specs->time_to_die, sim);
		printf("%li %i died\n", get_time() - philo->time_last_meal, philo->id);
		exit_end(sim);
	}
	else
	{
		while (!death(sim, philo) && philo->meals_count != 0)
		{
			if (!death(sim, philo) && !get_forks(sim, philo))
				break;
			if (!death(sim, philo))
				eating(sim, philo);
			else 
			 break;
			if (!death(sim, philo))
				sleeping(sim, philo);
			else 
				break;
			if (!death(sim, philo))
				thinking(sim, philo);
			else 
				break;
		}
	}
	return (0);
	exit_end(sim);
}

void *monitor(void *arg)
{
	static int i = -1;
	t_sim		*sim;
	t_philos	*philo;

	sim = (t_sim *)arg;
	philo = &sim->philos[++i];
	while (!death(sim, philo) && philo->meals_count != 0)
	{
		continue;
	}
	exit_end(sim);
	return (0);
}