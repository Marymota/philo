#include "philo.h"

int main (int argc, char *argv[])
{
	t_sim			*sim;

	error_handling(argc, argv);
	sim = init_sim(argc, argv);
	exit_end(sim);
}