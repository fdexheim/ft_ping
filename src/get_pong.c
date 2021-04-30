#include "../inc/ft_ping.h"

static void				init_msgdr(struct msghdr *msg, struct iovec *iov, struct sockaddr_in *addr, char *buffer)
{
	*addr = g_env->socket_data.addr_dest;

	iov->iov_base = g_env->in_buffer;
	iov->iov_len = g_env->full_packet_size;

	msg->msg_name = addr;
	msg->msg_namelen = sizeof(struct sockaddr_in);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	msg->msg_control = buffer;
	msg->msg_controllen = 64;
	msg->msg_flags = 0;
}

void					get_pong()
{
	ssize_t				ret;
	char				buffer[64];
	struct sockaddr_in	addr;
	struct iovec		iov;
	struct msghdr		msg;
	uint8_t				type = 42;

	while (type != ICMP_ECHOREPLY && type != ICMP_TIME_EXCEEDED)
	{
		ft_bzero(buffer, 64);
		ft_bzero(g_env->in_buffer, 4096);
		init_msgdr(&msg, &iov, &addr, buffer);
		ret = recvmsg(g_env->socket_data.sockfd, &msg, 0);
		type = ((struct icmphdr *)(g_env->in_buffer + g_env->ip_header_size))->type;
		if (gettimeofday(&g_env->run_data.time_end, NULL) < 0)
			printf("bad gettimeofday()\n");
		if (g_env->flags.v == true)
		{
			if (g_env->flags.verbose_level >= 2)
			{
				printf("Reveived packet :\n");
				dump_packet(g_env->in_buffer);
			}
		}
		check_response(&msg, ret);
		if (ret < 0)
		{
			if (g_env->flags.v == true)
				printf("[WARNING] recvmsg() failed or timed out\n");
			break ;
		}
	}
}
