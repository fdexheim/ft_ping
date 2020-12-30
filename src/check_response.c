#include "../inc/ft_ping.h"

static const char		*get_icmp_type_msg(uint8_t type)
{
	const char			*icmp_type_msg[] = {
		[ICMP_ECHOREPLY] = "Echo Reply",
		[1] = "Unknown type",
		[2] = "Unknown type",
		[ICMP_DEST_UNREACH] = "Destination Unreachable",
		[ICMP_SOURCE_QUENCH] = "Source Quench",
		[ICMP_REDIRECT] = "Redirect (change route)",
		[6] = "Unknown type",
		[7] = "Unknown type",
		[ICMP_ECHO] = "Echo Request",
		[ICMP_TIME_EXCEEDED] = "Time Exceeded",
		[ICMP_PARAMETERPROB] = "Parameter Problem",
		[ICMP_TIMESTAMP] = "Timestamp Request",
		[ICMP_TIMESTAMPREPLY] = "Timestamp Reply",
		[ICMP_INFO_REQUEST] = "Information Request",
		[ICMP_INFO_REPLY] = "Information Reply",
		[ICMP_ADDRESS] = "Address Mask Request",
		[ICMP_ADDRESSREPLY] = "Address Mask Reply",
	};

	if (type > 18)
		return ("Unknown type");
	return (icmp_type_msg[type]);
}

static void				check_icmp(void *icmp_ptr)
{
	uint8_t				type;
	uint8_t				code;

	type = ((struct icmphdr *)icmp_ptr)->type;
	code = ((struct icmphdr *)icmp_ptr)->code;

	printf("icmp type = %d (%s)\n" , type, get_icmp_type_msg(type));
	printf("icmp code = %d\n", code);
}

static void				check_checksums(void *full_packet)
{
	uint16_t			ip_sum;
	uint16_t			icmp_sum;

	ip_sum = calculate_checksum(full_packet, g_env->ip_header_size / 2);
	icmp_sum = calculate_checksum(full_packet + g_env->ip_header_size,
	(g_env->icmp_header_size + g_env->icmp_payload_size) / 2);
	if (g_env->flags.v == true && (ip_sum != 0 || icmp_sum != 0))
		printf("[WARNING] some checksums do not match :\n"\
		"ip checksum   = %x\nicmp checksum = %x\n",
		ip_sum, icmp_sum);
}

void		check_response()
{
	check_checksums(g_env->in_buffer);
	check_icmp(g_env->in_buffer + g_env->ip_header_size);
}
