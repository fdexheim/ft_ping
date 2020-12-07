#include "../inc/ft_ping.h"

void			recap()
{
	printf("RECAP (nyi)\n");
	close(g_env->socket_data.sockfd);
	exit(EXIT_SUCCESS);
}

void			sighandle(int sig)
{
	printf("Caught signal %d\n", sig);
	recap();
	exit(EXIT_SUCCESS);
}
