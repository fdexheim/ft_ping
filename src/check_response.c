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
		[9] = "Unknown type",
		[10] = "Unknown type",
		[ICMP_TIME_EXCEEDED] = "Time to live exceeded",
		[ICMP_PARAMETERPROB] = "Parameter Problem",
		[ICMP_TIMESTAMP] = "Timestamp Request",
		[ICMP_TIMESTAMPREPLY] = "Timestamp Reply",
		[ICMP_INFO_REQUEST] = "Information Request",
		[ICMP_INFO_REPLY] = "Information Reply",
		[ICMP_ADDRESS] = "Address Mask Request",
		[ICMP_ADDRESSREPLY] = "Address Mask Reply",
	};

	if (type > NR_ICMP_TYPES)
		return ("Unknown type");
	return (icmp_type_msg[type]);
}

uint8_t					check_icmp(void *icmp_ptr)
{
	uint8_t				type;

	type = ((struct icmphdr *)icmp_ptr)->type;
	return (type);
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
	suseconds_t			rtt;
	uint8_t				ttl;
	uint8_t				type;

	rtt = 0;
	ttl = ((struct iphdr *)g_env->in_buffer)->ttl;
	check_checksums(g_env->in_buffer);
	type = check_icmp(g_env->in_buffer + g_env->ip_header_size);
	printf("icmp_seq=%d ", g_env->run_data.current_iter);
	if (type == ICMP_ECHOREPLY)
	{
		rtt = get_rtt_sus(&g_env->run_data.time_new_iter,
			&g_env->run_data.time_end);
		printf("ttl=%d time=%ld.%ld ms\n", ttl, rtt / 1000, (rtt % 1000) / 100);
		record_statistics_success(rtt);
	}
	else
	{
		g_env->run_data.nb_packets_errors++;
		printf("%s\n", get_icmp_type_msg(type));
	}
	if (g_env->flags.c == true
		&& g_env->run_data.current_iter >= g_env->run_data.nb_iter)
		recap();
}
