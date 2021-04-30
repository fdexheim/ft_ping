#include "../inc/ft_ping.h"

void					init_headers(void *buffer)
{
	void				*ip_start;
	void				*icmp_start;
	void				*icmp_data_start;

	ip_start = buffer;
	icmp_start = buffer + g_env->ip_header_size;
	icmp_data_start = icmp_start + g_env->icmp_header_size;

	init_icmp_data(icmp_data_start);
	init_ip_header(ip_start);
	init_icmp_header(icmp_start);
}
