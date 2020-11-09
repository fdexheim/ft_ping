#include "../inc/ft_ping.h"

void		sighandle(int sig)
{
	printf("Caught signal %d\n", sig);
}
