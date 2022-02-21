#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <error.h>

typedef struct s_specs
{
	int n_of_philos;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int n_times_philos_must_eat;
}				t_specs;

typedef struct s_philos
{
	int				id;
	int				meals_count;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	left_fork;
	long int		time_last_meal;
}				t_philos;

typedef struct s_sim
{
	t_philos		*philos;
	t_specs			*specs;
	pthread_t		*threads;
	long int		start;
	int				end;
}				t_sim;

void		error_handling(int argc, char *argv[]);
t_sim		*init_sim(int argc, char *argv[]);
t_specs		*init_specs(int argc, char *argv[]);
t_philos	*init_philos(t_sim *sim, long int start);
void		init_forks(t_philos *philo, t_specs *specs);
void		init_threads(t_sim *sim);
void		*action(void *arg);
long int	get_time(void);
void		free_structs(t_sim *sim);
void		ft_usleep (long int time);
int			ft_atoi(const char *n);
int			ft_isdigit(int c);
int			exit_error(t_sim *sim, char *err);
int			exit_end(t_sim *sim);
void		init_mutex(t_sim *sim);
void		init_threads(t_sim *sim);
int			death(t_sim *sim, t_philos *philo);

#endif