#include "philo.h"

/* Check if the number of arguments are valid */
void validate_arguments(int argc, char *argv[])
{
	int i;

	i = 0;
	if (argc > 6)
	{
		perror("Too many arguments");
		exit(EXIT_FAILURE);
	}
	else if (argc < 5)
	{
		perror("Not enough arguments");
		exit(EXIT_FAILURE);	
	}
	while (argv[++i])
	{
		if (atoi(argv[i]) <= 0)
		{
			printf("%i has to be more than 0", atoi(argv[i]));
			exit(EXIT_FAILURE);	
		}
	}
}
