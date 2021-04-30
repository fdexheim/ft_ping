#include "../inc/ft_ping.h"

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
		[ICMP_ADDRESSREPLY] = "Address Mask Reply"
	};

	if (type > NR_ICMP_TYPES)
		return ("Unknown type");
	return (icmp_type_msg[type]);
}

void					check_response(struct msghdr *hdr, ssize_t read_size)
{
	suseconds_t			rtt;
	uint8_t				ttl;
	uint8_t				type;
	struct sockaddr_in	*addr = hdr->msg_name;
	char				addr_str[100];

	type = ((struct icmphdr *)(g_env->in_buffer + g_env->ip_header_size))->type;
	check_checksums(g_env->in_buffer);
	if (type == ICMP_ECHOREPLY)
	{
		ttl = ((struct iphdr *)g_env->in_buffer)->ttl;
		rtt = get_rtt_sus(&g_env->run_data.time_new_iter, &g_env->run_data.time_end);
		printf("%ld bytes from %s", read_size - g_env->ip_header_size, g_env->dest);
		if (ft_strcmp(g_env->dest, g_env->addr_str))
			printf(" (%s)", g_env->addr_str);

g_env->run_data.nb_packets_received++;
		printf(": icmp_seq=%d ", g_env->run_data.current_iter);
		printf("ttl=%d time=%ld.%.02ld ms\n", ttl, rtt / 1000, (rtt % 1000) / 10);
		record_statistics_success(rtt);
	}
	else
	{
		if (type == ICMP_ECHO)
			return ;
		ft_bzero(addr_str, 100);
		inet_ntop(AF_INET, &addr->sin_addr,
			addr_str, 99);
		printf("From %s ", addr_str);
		g_env->run_data.nb_packets_errors++;
		printf("%s\n", get_icmp_type_msg(type));
	}
}
