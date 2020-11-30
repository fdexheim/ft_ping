#include "../inc/ft_ping.h"

void					init_headers()
{
printf(">>init_headers called\n");
	void				*ip_start;
	void				*icmp_start;

	ip_start = g_env->out_buffer;
	icmp_start = g_env->out_buffer + g_env->ip_header_size;
	init_ip_header(ip_start);
	init_icmp_header(icmp_start);
	printf("ip_header_size = %ld\nicmp_header_size = %ld\nicmp_payload size = %ld\nfull packet size = %ld\n", g_env->ip_header_size, g_env->icmp_header_size, g_env->icmp_payload_size, g_env->full_packet_size);
	printf("buffer_start = %p | iphdr = %p | icmphdr = %p\n", g_env->out_buffer, ip_start, icmp_start);
}
