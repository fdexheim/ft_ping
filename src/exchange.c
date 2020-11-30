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

//	ret = recvmsg(g_env->socket_data.sockfd, (struct msghdr *)g_env->in_buffer, 0);
	ret = read(g_env->socket_data.sockfd, g_env->in_buffer, g_env->full_packet_size);
	if (ret < 0)
		printf("Bad recvmsg\n");
	printf("Received %ld bytes\n", ret);
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
}
