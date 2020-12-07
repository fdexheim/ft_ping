#include "../inc/ft_ping.h"

void					init_headers()
{
	void				*ip_start;
	void				*icmp_start;

	ip_start = g_env->out_buffer;
	icmp_start = g_env->out_buffer + g_env->ip_header_size;
	init_ip_header(ip_start);
	init_icmp_header(icmp_start);
}
