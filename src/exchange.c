#include "../inc/ft_ping.h"

static void					println_status(suseconds_t diff_usec)
{
	printf("%ld bytes from %s (%s): ",
		g_env->icmp_header_size + g_env->icmp_payload_size, g_env->dest,
		g_env->addr_str);
	printf("icmp_seq=%d ttl=%d ", g_env->run_data.current_iter, 115);
	printf("time=%ld.%ld ms\n", diff_usec / 1000, (diff_usec % 1000) / 100);
}

static void					record_statistics(struct timeval *start,
	struct timeval *end)
{
	time_t					diff_sec;
	suseconds_t				diff_usec;

	diff_sec = end->tv_sec - start->tv_sec;
	diff_usec = 1000000 * diff_sec + (end->tv_usec - start->tv_usec);
	if (g_env->run_data.current_iter == 1)
	{
		g_env->run_data.min = diff_usec;
		g_env->run_data.avg = diff_usec;
		g_env->run_data.max = diff_usec;
		g_env->run_data.mdev = 0;
	}
	else
	{
		if (diff_usec < g_env->run_data.min)
			g_env->run_data.min = diff_usec;
		if (diff_usec > g_env->run_data.max)
			g_env->run_data.max = diff_usec;
		g_env->run_data.sum += diff_usec;
	}
	println_status(diff_usec);
}

/*
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
*/

static void				give_ping()
{
	ssize_t				ret;

	ret = sendto(g_env->socket_data.sockfd, g_env->out_buffer,
		g_env->full_packet_size, 0, NULL, 0);
	if (g_env->flags.v == true && ret > 0)
	{
		printf("Sending packet :\n");
		dump_packet(g_env->out_buffer);
	}
	if (ret < 0 && g_env->flags.v == true)
		printf("[WARNING] sendto() failed\n");
	g_env->run_data.nb_packets_sent++;
}

static void				get_pong()
{
	ssize_t				ret;

	ret = read(g_env->socket_data.sockfd, g_env->in_buffer,
		g_env->full_packet_size);
	if (g_env->flags.v == true && ret > 0)
	{
		printf("Received packet :\n");
		dump_packet(g_env->in_buffer);
	}
	gettimeofday(&g_env->run_data.time_end, NULL);
	if (ret < 0)
	{
		if (g_env->flags.v == true)
			printf("[WARNING] read() failed\n");
	}
	else
	{
		g_env->run_data.nb_packets_received++;
	}
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
	g_env->run_data.current_iter++;
	ft_bzero(g_env->out_buffer, 4096);
	ft_bzero(g_env->in_buffer, 4096);
	init_icmp_data(g_env->out_buffer + g_env->ip_header_size + g_env->icmp_header_size);
	init_headers();
	give_ping();
	get_pong();
	check_checksums(g_env->in_buffer);
	record_statistics((struct timeval *)(g_env->out_buffer + g_env->ip_header_size
		+ g_env->icmp_header_size), &g_env->run_data.time_end);
}
