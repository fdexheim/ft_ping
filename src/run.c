#include "../inc/ft_ping.h"

static void				exchange()
{
	usleep(1);
}

static void				set_addr(char *addr)
{
	
}

static int32_t			create_socket()
{
	g_env->socket_data.sock = socket(AF_INET, SOCK_STREAM, );
}

static int32_t			setup_connection()
{
	
}

void					run(void)
{
	uint32_t			i;
	struct timeval		start_iter;
	struct timeval		end_iter;
	struct timeval		diff;

	i = 0;
	signal(SIGINT, sighandle);
	printf("PING %s (%s) %d(%d) bytes of data.\n",
		g_env->addr, g_env->addr, 1, 1);
	while (i < g_env->run_data.nb_iter)
	{
		printf("loop iter %d\n", i + 1);
		gettimeofday(&start_iter, NULL);
		exchange();
		gettimeofday(&end_iter, NULL);
		diff.tv_sec = end_iter.tv_sec - start_iter.tv_sec;
		diff.tv_usec = end_iter.tv_usec - start_iter.tv_usec;
		printf("time start = %ld.%ld | time end = %ld.%ld | diff = %ld.%ld\n",
			start_iter.tv_sec, start_iter.tv_usec,
			end_iter.tv_sec, end_iter.tv_usec,
			diff.tv_sec, diff.tv_usec);
		i++;
	}
}
