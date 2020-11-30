#include "../inc/ft_ping.h"

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
	printf(" === IPV4 DUMP === \n");
	printf("version = %d | ihl = %d | tos = %d | tot_len = %d\n", ptr->version, ptr->ihl, ptr->tos, ptr->tot_len);
	printf("id = %d | frag_off = %d\n", ptr->id, ptr->frag_off);
	printf("ttl = %d | protocol = %d, check = %d\n", ptr->ttl, ptr->protocol, ptr->check);
	printf("saddr = %d | daddr = %d\n", ptr->saddr, ptr->daddr);
}

void			dump_ip(void *ip_start)
{
	if (g_env->flags.ipv6 == true)
		dump_ipv6(ip_start);
	else
		dump_ipv4(ip_start);
}
