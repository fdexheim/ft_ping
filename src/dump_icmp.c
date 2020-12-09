#include "../inc/ft_ping.h"

static const char		*icmp4_format =
"+--------------------------[ ICMP HEADER ]---------------------------+\n"
"| type %9x | code %9x | checksum %23x |\n"
"+--------------------------------------------------------------------+\n"
"| id %28x | sequence %23x |\n"
"+--------------------------------------------------------------------+\n";

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
	printf(icmp4_format, ptr->type, ptr->code, ptr->checksum,
		ptr->un.echo.id, ptr->un.echo.sequence);
}

static void				dump_icmp_data(void *data_start)
{
	struct timeval		*tv;

	tv = data_start;
	printf("icmp timestamp data - time = %ld.%ld\n", tv->tv_sec, tv->tv_usec);
}

void					dump_icmp(void *icmp_start)
{
	if (g_env->flags.ipv6 == true)
		dump_icmpv6(icmp_start);
	else
		dump_icmpv4(icmp_start);
	dump_icmp_data(icmp_start + g_env->icmp_header_size);
}
