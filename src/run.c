#include "../inc/ft_ping.h"

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

	for (uint32_t i = 0; i < g_env->run_data.nb_iter; i++)
	{
		ft_bzero(g_env->out_buffer, 4096);
		ft_bzero(g_env->in_buffer, 4096);
		init_icmp_data(g_env->out_buffer + g_env->ip_header_size + g_env->icmp_header_size);
		init_headers();
		exchange();
	}
	sighandle(SIGINT);
}
