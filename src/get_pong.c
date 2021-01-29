#include "../inc/ft_ping.h"

static void				print_origin(void *src, ssize_t size, ssize_t read_size)
{
	(void)size;
/*	int					ret;
	char				addr_str[100];
	struct addrinfo		*ptr;
	struct addrinfo		hints = {
		.ai_family = g_env->flags.ipv6 == true ? AF_INET6 : AF_INET,
		.ai_flags = 0,
		.ai_protocol = IPPROTO_ICMP,
		.ai_socktype = SOCK_RAW
	};
*/
	if (((struct sockaddr_in*)src)->sin_addr.s_addr
		== g_env->socket_data.addr_dest.sin_addr.s_addr)
	{
		printf("%ld bytes from %s (%s): ",
			read_size, g_env->dest, g_env->addr_str);
	}
	else
	{
//		ft_bzero(addr_str, 100);
//		ret = getaddrinfo(, NULL, &hints, &ptr);
//		inet_ntop(ptr->ai_addr->sa_family, ptr->ai_addr->sa_data + 2,
//			addr_str, 99);
		printf("From %s (%s) ", g_env->addr_str, g_env->addr_str);
	}
}

void					get_pong()
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
	print_origin(&msg.msg_name, msg.msg_namelen, ret);
	if (gettimeofday(&g_env->run_data.time_end, NULL) < 0)
		printf("bad gettimeofday()");
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
	check_response();
}
