#include "../inc/ft_ping.h"

void				get_pong()
{
	ssize_t				ret;
	char				buffer[256];
	struct sockaddr_in	addr = g_env->socket_data.addr_dest;
	struct iovec		iov = {
		.iov_base = g_env->in_buffer,
		.iov_len = g_env->full_packet_size
	};
	struct msghdr		msg = {
		.msg_name = &addr,
		.msg_namelen = sizeof(struct sockaddr_in),
		.msg_iov = &iov,
		.msg_iovlen = 1,
		.msg_control = buffer,
		.msg_controllen = 256,
		.msg_flags = MSG_ERRQUEUE
	};

	ft_bzero(buffer, 256);
	ft_bzero(g_env->in_buffer, 4096);
	ret = recvmsg(g_env->socket_data.sockfd, &msg, 0);
	if (g_env->flags.v == true)
	{
		printf("recvmsg() ret = %ld\n", ret);
		if (g_env->flags.verbose_level >= 2)
		{
			printf("Reveived packet :\n");
			dump_packet(g_env->in_buffer);
		}
	}
	if (ret < 0)
	{
		if (g_env->flags.v == true)
			printf("[WARNING] recvmsg() failed\n");
	}
	else
		g_env->run_data.nb_packets_received++;
	gettimeofday(&g_env->run_data.time_end, NULL);
	check_response();
}
