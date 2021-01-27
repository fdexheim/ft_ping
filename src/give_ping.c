#include "../inc/ft_ping.h"

void				give_ping()
{
	ssize_t				ret;

	ft_bzero(g_env->out_buffer, 4096);
	init_icmp_data(g_env->out_buffer + g_env->ip_header_size + g_env->icmp_header_size);
	init_headers(g_env->out_buffer);
	ret = sendto(g_env->socket_data.sockfd, g_env->out_buffer,
		g_env->full_packet_size, 0,
		(struct sockaddr *)&g_env->socket_data.addr_dest,
		sizeof(g_env->socket_data.addr_dest));

	if (g_env->flags.v == true)
	{
		printf("sento() ret = %ld\n", ret);
		if (g_env->flags.verbose_level >= 2)
		{
			printf("Sent packet :\n");
			dump_packet(g_env->out_buffer);
		}
	}
	if (ret < 0)
	{
		if (g_env->flags.v == true)
			printf("[WARNING] sendto() failed\n");
	}
	else
		g_env->run_data.nb_packets_sent++;
	alarm(1);
}
