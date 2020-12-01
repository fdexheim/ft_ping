#include "../inc/ft_ping.h"

static void				dump_brute(void *buff)
{
	unsigned char *ptr = buff;

	for (uint32_t i = 0; i < g_env->full_packet_size; i++)
	{
		if (i % 16 == 0)
			printf("\n");
		printf("%3hd ", ptr[i]);
	}
	printf("\n");
}

static void				give_ping()
{
	ssize_t				ret;

	ret = sendto(g_env->socket_data.sockfd, g_env->out_buffer,
	g_env->full_packet_size, 0, NULL, 0);
	printf("Sent %ld bytes\n", ret);
}

static void				get_pong()
{
	ssize_t				ret;

	ret = read(g_env->socket_data.sockfd, g_env->in_buffer, g_env->full_packet_size);
	if (ret < 0)
		printf("Bad recvmsg\n");
	printf("Received %ld bytes\n", ret);
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
		"ip checksum   = %x\n icmp checsum = %x\n",
		ip_sum, icmp_sum);
}

void					exchange()
{
	printf("\n===== OUT DATA =====\n");
	dump_packet(g_env->out_buffer);
	dump_brute(g_env->out_buffer);
	give_ping();
	get_pong();
	printf("\n===== IN DATA =====\n");
	dump_packet(g_env->in_buffer);
	dump_brute(g_env->in_buffer);
	check_checksums(g_env->in_buffer);
}
