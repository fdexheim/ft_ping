#include "../inc/ft_ping.h"

static void					iter_step(int s)
{
	signal(SIGALRM, iter_step);
	alarm(1);
	(void)s;
	exchange();
	if (g_env->run_data.current_iter >= g_env->run_data.nb_iter)
		recap();
}

void					run(void)
{
	if (getuid() != 0)
	{
		printf("Make sure you're running the program with the "
			"requirered permissions (SU) to open a raw socket\n");
		exit(EXIT_FAILURE);
	}
	if (setup_socket() < 0)
	{
		printf("ping: %s: Name or service unknown\n", g_env->addr_str);
		return ;
	}
	signal(SIGINT, sighandle);
	printf("PING %s (%s) %ld(%ld) bytes of data.\n",
			g_env->dest, g_env->addr_str, g_env->icmp_payload_size,
			g_env->full_packet_size);
	gettimeofday(&g_env->run_data.time_start, NULL);
	iter_step(42);
	while (1) {

	}
}
