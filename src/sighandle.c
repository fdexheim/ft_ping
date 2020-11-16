#include "../inc/ft_ping.h"

static void		recap()
{

}

void			sighandle(int sig)
{
	printf("Caught signal %d\n", sig);
	if (sig == SIGINT)
		recap();
}
