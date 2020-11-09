#include "../inc/ft_ping.h"

static void		exchange()
{
	
}

void			run(void)
{
	uint32_t	i;

	i = 0;
	signal(SIGINT, sighandle);
	printf("PING %s (%s) %d(%d) bytes of data.\n",
		g_env->addr, g_env->addr, 1, 1);
	while (i < g_env->run_data.nb_iter)
	{
		printf("loop iter");
		exchange();
		i++;
	}
}
