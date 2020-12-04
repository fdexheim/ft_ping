#include "../inc/ft_ping.h"

static void		recap()
{
	printf("RECAP (nyi)\n");
}

void			sighandle(int sig)
{
	printf("Caught signal %d\n", sig);
	if (sig == SIGINT)
		recap();
	close(g_env->socket_data.sockfd);
	exit(EXIT_SUCCESS);
}
