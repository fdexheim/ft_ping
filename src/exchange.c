#include "../inc/ft_ping.h"

static void					record_statistics(struct timeval *start,
	struct timeval *end)
{
	time_t					diff_sec;
	suseconds_t				diff_usec;

	printf("start sec = %ld usec = %ld | end sec = %ld usec = %ld\n", start->tv_sec, start->tv_usec, end->tv_sec, end->tv_usec);
	diff_sec = end->tv_sec - start->tv_sec;
	diff_usec = 1000000 * diff_sec + (end->tv_usec - start->tv_usec);
//	diff_msec = 0;
//	diff_msec += ((end->tv_sec - start->tv_sec) * 1000);
//	diff_msec += ((end->tv_usec - start->tv_usec) / 1000);
	printf("delay = %ld us\n", diff_usec);
}

void					dump_brute(void *buff)
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
	g_env->run_data.nb_packets_sent++;
}

static void				get_pong()
{
	ssize_t				ret;

	ret = read(g_env->socket_data.sockfd, g_env->in_buffer,
		g_env->full_packet_size);
	if (ret < 0)
		printf("Read failed\n");
	printf("Received %ld bytes\n", ret);
	g_env->run_data.nb_packets_received++;
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
	struct timeval			cmp;
	g_env->run_data.current_iter++;
	ft_bzero(g_env->out_buffer, 4096);
	ft_bzero(g_env->in_buffer, 4096);
	init_icmp_data(g_env->out_buffer + g_env->ip_header_size + g_env->icmp_header_size);
	init_headers();
//	printf("\n===== OUT DATA =====\n");
//	dump_packet(g_env->out_buffer);
//	dump_brute(g_env->out_buffer);
	give_ping();
	get_pong();
	gettimeofday(&cmp, NULL);
//	printf("\n===== IN DATA =====\n");
//	dump_packet(g_env->in_buffer);
//	dump_brute(g_env->in_buffer);
	check_checksums(g_env->in_buffer);
	record_statistics((struct timeval *)(g_env->out_buffer + g_env->ip_header_size
		+ g_env->icmp_header_size), &cmp);
}
