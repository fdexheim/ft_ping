#include "../inc/ft_ping.h"

static void				print_origin(void *msg_name, size_t msg_namelen, ssize_t read_size)
{
	uint8_t				type;
	struct sockaddr_in	*addr = msg_name;
	char				addr_str[100];

	(void)msg_namelen;

	type = check_icmp(g_env->in_buffer + g_env->ip_header_size);
	if (type == ICMP_ECHOREPLY)
	{
		printf("%ld bytes from %s (%s): ",
			read_size - g_env->ip_header_size, g_env->dest, g_env->addr_str);
	}
	else
	{
		ft_bzero(addr_str, 100);
		inet_ntop(AF_INET, &addr->sin_addr,
			addr_str, 99);
		printf("From %s (%s) ", addr_str, addr_str);
	}
}

void					get_pong()
{
	ssize_t				ret;
	char				buffer[64];
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
		.msg_controllen = 64,
		.msg_flags = MSG_ERRQUEUE
	};

	ft_bzero(buffer, 64);
	ft_bzero(g_env->in_buffer, 4096);
	ret = recvmsg(g_env->socket_data.sockfd, &msg, 0);
	print_origin(msg.msg_name, msg.msg_namelen, ret);
	if (gettimeofday(&g_env->run_data.time_end, NULL) < 0)
		printf("bad gettimeofday()\n");
	if (g_env->flags.v == true)
	{
		if (g_env->flags.verbose_level >= 2)
		{
			printf("Reveived packet :\n");
			dump_packet(g_env->in_buffer);
			dump_msghdr(&msg);
		}
	}
	if (ret < 0)
	{
		if (g_env->flags.v == true)
			printf("[WARNING] recvmsg() failed\n");
	}
	check_response();
}
