#include "../inc/ft_ping.h"

void					init_icmp_data(void *data_start)
{
	printf(">>init_icmp_data called\n");
	struct timeval		*tv;

	tv = data_start;
	if (gettimeofday(tv, NULL) == -1)
		printf("gettime of day failed");
}
