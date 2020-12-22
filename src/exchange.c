#include "../inc/ft_ping.h"

/*
void					dump_brute(void *buff)
{
	unsigned char *ptr = buff;

	for (uint32_t i = 0; i < g_env->full_packet_size; i++)
	{
		if (i % 16 == 0)
			printf("\n");
		printf("%3hd ", ptr[i]);
	}
	printf("\n");
}
*/

static void				give_ping()
{
	ssize_t				ret;

	printf("giving ping\n");
	ret = sendto(g_env->socket_data.sockfd, g_env->out_buffer,
		g_env->full_packet_size, MSG_CONFIRM, NULL, 0);
	printf("give ping ret = %ld\n", ret);
	if (g_env->flags.v == true)
	{
		printf("Sending packet :\n");
		dump_packet(g_env->out_buffer);
	}
	if (ret < 0 && g_env->flags.v == true)
		printf("[WARNING] sendto() failed\n");
	g_env->run_data.nb_packets_sent++;
}

static void				get_pong()
{
	ssize_t				ret;

	printf("getting pong\n");
	ret = read(g_env->socket_data.sockfd, g_env->in_buffer,
		g_env->full_packet_size);
	printf("get pong ret = %ld\n", ret);
	if (g_env->flags.v == true && ret > 0)
	{
		printf("Received packet :\n");
		dump_packet(g_env->in_buffer);
	}
	gettimeofday(&g_env->run_data.time_end, NULL);
	if (ret < 0)
	{
		if (g_env->flags.v == true)
			printf("[WARNING] read() failed\n");
	}
	else
	{
		g_env->run_data.nb_packets_received++;
	}
}

void					exchange()
{
	suseconds_t			rtt;

	g_env->run_data.current_iter++;
	ft_bzero(g_env->out_buffer, 4096);
	ft_bzero(g_env->in_buffer, 4096);
	init_icmp_data(g_env->out_buffer + g_env->ip_header_size + g_env->icmp_header_size);
	init_headers();
	give_ping();
	get_pong();
	check_response();
	rtt = get_rtt_sus((struct timeval *)(g_env->out_buffer + g_env->ip_header_size + g_env->icmp_header_size), &g_env->run_data.time_end);
	record_statistics_success(rtt);
}
