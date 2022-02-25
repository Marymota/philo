/* 	1 second = 1000 milliseconds 
	-> tv_sec (second) = milliseconds / 1000; 
	1 microsecond = 0.001 milliseconds 
	-> tv_usec (microsecond) = milliseconds * 1000;
	If you need to count time using float values 
	tv_sec and tv_usec must be summed up;

	usleep() suspends execution for microseconds
*/

#include <philo.h>

long int	get_time(void)
{
	long int		ms;
	struct timeval	time;

	ms = 0;
	if (gettimeofday(&time, NULL) != 0)
	{
		perror("Can't get current time");
		exit(EXIT_FAILURE);
	}
	ms = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (ms);
}

void	ft_usleep (long int time)
{
	long int	start;

	start = 0;
	start = get_time();
	while ((get_time() - start) < time)
		usleep(time / 10);
}
