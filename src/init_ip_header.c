#include "../inc/ft_ping.h"

static void				init_ipv6_header(void *header_start)
{
	(void)header_start;
}

static void				init_ipv4_header(void *header_start)
{
	struct iphdr		*hdr = (struct iphdr *)header_start;

	hdr->version = 4;
	hdr->ihl = 5;
	hdr->tos = 0;
	hdr->tot_len = htons(g_env->full_packet_size);
	hdr->id = 0;
	hdr->frag_off = 0;
	if (g_env->flags.t == false)
		hdr->ttl = 64;
	else
		hdr->ttl = g_env->run_data.forced_ttl;
	hdr->protocol = 1;
	hdr->check = 0;
	hdr->saddr = INADDR_ANY;
	hdr->daddr = g_env->socket_data.addr_dest.sin_addr.s_addr;
	hdr->check = calculate_checksum(header_start, 10);
}

void					init_ip_header(void *header_start)
{
	if (g_env->flags.ipv6 == true)
		init_ipv6_header(header_start);
	else
		init_ipv4_header(header_start);
}
