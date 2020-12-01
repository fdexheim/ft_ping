#include "../inc/ft_ping.h"

static void				dump_icmpv6(void *icmp_start)
{
	struct icmp6_hdr	*ptr;

	ptr = icmp_start;
	(void)ptr;
}

static void				dump_icmpv4(void *icmp_start)
{
	struct icmphdr		*ptr;

	ptr = icmp_start;
	printf(" === ICMP HEADER DUMP ===\n");
	printf("type = %d | code = %d | checksum = %x\n", ptr->type, ptr->code, ptr->checksum);
	printf("id = %d | sequence = %d\n", ptr->un.echo.id, ptr->un.echo.sequence);
}

static void				dump_icmp_data(void *data_start)
{
	printf(" === ICMP DATA DUMP ===\n");
	struct timeval		*tv;

	tv = data_start;
	printf("time = %ld.%ld\n", tv->tv_sec, tv->tv_usec);
}

void					dump_icmp(void *icmp_start)
{
	if (g_env->flags.ipv6 == true)
		dump_icmpv6(icmp_start);
	else
		dump_icmpv4(icmp_start);
	dump_icmp_data(icmp_start + g_env->icmp_header_size);
}
