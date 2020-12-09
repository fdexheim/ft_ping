#include "../inc/ft_ping.h"

static const char *ipv4_format = \
"+--------------------------[ IPV4 HEADER ]---------------------------+\n"
"| version %6x | ihl %10x | tos %11x | tot_len %6hx |\n"
"+--------------------------------------------------------------------+\n"
"| id %28x | frag_off %23x |\n"
"+--------------------------------------------------------------------+\n"
"| ttl %10x | protocol %5x | check %26x |\n"
"+--------------------------------------------------------------------+\n"
"| saddr %25x | daddr %26x |\n"
"+--------------------------------------------------------------------+\n";

void			dump_ipv6(void *ip_start)
{
	struct ip6_hdr		*ptr;

	ptr = ip_start;
	(void)ptr;
}

void			dump_ipv4(void *ip_start)
{
	struct iphdr		*ptr;

	ptr = ip_start;
	printf(ipv4_format, ptr->version, ptr->ihl, ptr->tos, ptr->tot_len,
	ptr->id, ptr->frag_off, ptr->ttl, ptr->protocol, ptr->check, ptr->saddr,
	ptr->daddr);
}

void			dump_ip(void *ip_start)
{
	if (g_env->flags.ipv6 == true)
		dump_ipv6(ip_start);
	else
		dump_ipv4(ip_start);
}
