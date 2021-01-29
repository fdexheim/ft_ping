#include "../inc/ft_ping.h"

void					init_icmp_data(void *data_start)
{
	unsigned char		*ptr;

	ptr = data_start;
	for (int i = 0; i < FULL_PACKET_SIZE - (IP_HEADER_SIZE + ICMP_HEADER_SIZE); i++)
	{
		ptr[i] = 42;
	}
}
