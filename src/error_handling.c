#include "philo.h"

void error_handling(int argc, char *argv[])
{
	if ((argc < 5 && argc != 6))
	{
		printf("not enough arguments\n");
		exit(EXIT_FAILURE);
	}
	if (argc > 6 )
	{
		printf("too many arguments\n");
		exit(EXIT_FAILURE);
	}
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[1]) > 200)
	{
		printf("invalid number of philosophers\n");
		exit(EXIT_FAILURE);
	}
	if (ft_atoi(argv[2]) < 0.60 || ft_atoi(argv[3]) < 0.60)
	{
		printf("invalid number of philosophers\n");
		exit(EXIT_FAILURE);
	}

}