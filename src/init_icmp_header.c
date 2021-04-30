#include "../inc/ft_ping.h"

static void				init_icmp_header_ipv6(void *header_start)
{
	struct icmp6_hdr	*hdr = (struct icmp6_hdr *)header_start;

	(void)hdr;
//	hdr->type = ICMP_ECHO_REQUEST; // 8 is code for ping request in icmp FOR IPV4 ! is == 128 for ipv6
	
}

static void				init_icmp_header_ipv4(void *header_start)
{
	struct icmphdr		*hdr = (struct icmphdr *)header_start;

	hdr->type = ICMP_ECHO;
	hdr->code = 0;
	hdr->un.echo.id = htons(42);
	hdr->un.echo.sequence = htons(g_env->run_data.current_iter);
	hdr->checksum = 0;
	hdr->checksum = calculate_checksum(header_start,
		(g_env->icmp_header_size + g_env->icmp_payload_size) / 2);
}

void					init_icmp_header(void *header_start)
{
	if (g_env->flags.ipv6 == true)
		init_icmp_header_ipv6(header_start);
	else
		init_icmp_header_ipv4(header_start);
}
