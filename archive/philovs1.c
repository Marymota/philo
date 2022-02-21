#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_settings
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;		
}	t_settings;

typedef struct s_philo
{
	int id;
	int last_meal;
	int number_of_meals;
}			t_philo;


void validate_number_of_arguments(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf("Invalid number of arguments: %d. Should be 4 or 5.\n", argc - 1);
		exit(EXIT_FAILURE);
	}
}

void *routine(t_philo *philo)
{
	pthread_mutex_t mutex_left;
	pthread_mutex_t mutex_right;
	pthread_mutex_lock(&mutex_left);
	printf("philo %d got the left fork\n", philo->id);
	pthread_mutex_lock(&mutex_right);
	printf("philo %d got the right fork\n", philo->id);
	printf("philo %d is eating", philo->id);
	pthread_mutex_unlock(&mutex_left);
	pthread_mutex_unlock(&mutex_right);
	printf("philo %d is thinking\n", philo->id);
	printf("philo %d is sleeping\n", philo->id);
	return (NULL);
}

static void initiate_philosophers(t_settings *data)
{
	int *res;
	pthread_t philo[data->number_of_philosophers];
	
	pthread_mutex_t mutex;
	int i = -1;
	pthread_mutex_init(&mutex, NULL);
	while (++i < data->number_of_philosophers)
	{
		if (pthread_create(philo + i, NULL, (void *) &routine, NULL) != 0)
		{
			perror("Failed to create thread!");
			exit(EXIT_FAILURE);
		}
		printf("Thread %d has started\n", i);
	} 
	i = -1;
	while (++i < data->number_of_philosophers)
	{
		if (pthread_join(philo[i], (void **) &res) != 0)
			exit(EXIT_FAILURE);
		printf("Thread %d has finished execution\n", i);
	}
	printf("Number of thinking: %d\n", *res);
	exit(EXIT_SUCCESS);
}

// Initiate all arguments in the settings struct
static void	initiate_simulation(t_settings *data, char *argv[])
{
	if (!argv[1] || !argv[2] || !argv[3] || !argv[4])
		exit(EXIT_FAILURE);
	data->number_of_philosophers = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	if (!argv[5])
		data->number_of_times_each_philosopher_must_eat = -1;
	else	
		data->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	if (data->number_of_philosophers < 0 || data->time_to_die < 0 || \
	data->time_to_eat < 0 || data->time_to_sleep < 0)
		exit(EXIT_FAILURE);
}

int main (int argc, char *argv[])
{
	t_settings data;

	memset(&data, 0, sizeof(data));
	validate_number_of_arguments(argc);
	initiate_simulation(&data, argv);
	initiate_philosophers(&data);
}
