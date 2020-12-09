#include "../inc/ft_ping.h"

void				recap()
{
	int32_t			packet_ratio;
	time_t			span_sec;
	suseconds_t		total_span;

	span_sec = g_env->run_data.time_end.tv_sec
		- g_env->run_data.time_start.tv_sec;
	total_span = (span_sec * 1000000 + g_env->run_data.time_end.tv_usec
		- g_env->run_data.time_start.tv_usec) / 1000;
	packet_ratio = 100 - 100 * (g_env->run_data.nb_packets_received
		/ g_env->run_data.nb_packets_sent);
	printf("\n--- %s (%s) ping statistics ---\n", g_env->dest, g_env->addr_str);
	printf("%d packets transmitted, %d received, ",
		g_env->run_data.nb_packets_sent, g_env->run_data.nb_packets_received);
	printf("%d%% packet loss, time %ld ms\n", packet_ratio, total_span);
	printf("rtt min/avg/max/mdec = %d/%d/%d/%d ms\n", 42, 42, 42, 42);
	close(g_env->socket_data.sockfd);
	exit(EXIT_SUCCESS);
}

void			sighandle(int sig)
{
	(void)sig;
	recap();
	exit(EXIT_SUCCESS);
}
