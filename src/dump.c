#include "../inc/ft_ping.h"

void					dump_addrinfo(struct addrinfo *ptr)
{
	printf("ai_flags = %d\n", ptr->ai_flags);
	printf("ai_family = %d\n", ptr->ai_family);
	printf("ai_socktype = %d\n", ptr->ai_socktype);
	printf("ai_protocol = %d\n", ptr->ai_protocol);
	printf("ai_addrlen = %d\n", ptr->ai_addrlen);
	printf("ai_canonname = %s\n", ptr->ai_canonname);
}

void					dump_addrinfo_list(struct addrinfo *start)
{
printf(">>debug_addrinfo_list called\n");
	struct addrinfo		*ptr;

	ptr = start;
	while (ptr != NULL)
	{
		dump_addrinfo(ptr);
		ptr = ptr->ai_next;
	}
}

void					dump_packet(void *packet)
{
	void				*ip_start;
	void				*icmp_start;

	ip_start = packet;
	icmp_start = packet + g_env->ip_header_size;
	dump_ip(ip_start);
	dump_icmp(icmp_start);
}

void					dump_env()
{
	
}
