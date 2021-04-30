#include "../inc/ft_ping.h"

void					iter_step(int s)
{
	(void)s;

	g_env->run_data.current_iter++;
	alarm(1);
	give_ping();
	get_pong();
	if (g_env->flags.c == true && g_env->run_data.current_iter >= g_env->run_data.nb_iter)
	{
		alarm(0);
		recap(42);
	}
}

static void				setup_sigs()
{
	if ((signal(SIGINT, recap) == SIG_ERR)
		|| (signal(SIGALRM, iter_step) == SIG_ERR))
	{
		printf("Bad signal() setup\n");
		exit(EXIT_FAILURE);
	}
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
	printf("PING %s (%s) %ld(%ld) bytes of data.\n",
			g_env->dest, g_env->addr_str, g_env->icmp_payload_size,
			g_env->full_packet_size);
	gettimeofday(&g_env->run_data.time_start, NULL);
	setup_sigs();
	iter_step(42);
	while (1) {

	}
}
