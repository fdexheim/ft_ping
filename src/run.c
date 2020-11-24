#include "../inc/ft_ping.h"

static void					init_size_values()
{
	if (g_env->flags.ipv6 == true)
	{
		g_env->ip_header_size = IP_HEADER_SIZE_V6;
		g_env->icmp_header_size = ICMP_HEADER_SIZE_V6;
		g_env->icmp_payload_size = ICMP_PAYLOAD_SIZE_V6;
	}
	else
	{
		g_env->ip_header_size = IP_HEADER_SIZE;
		g_env->icmp_header_size = ICMP_HEADER_SIZE;
		g_env->icmp_payload_size = ICMP_PAYLOAD_SIZE;
	}
	g_env->full_packet_size = g_env->ip_header_size + g_env->icmp_header_size
		+ g_env->icmp_payload_size;

}


static void				exchange()
{
	ssize_t				ret;

	dump_packet(g_env->out_buffer);
	printf("End of dump packet\n");
	ret = sendto(g_env->socket_data.sockfd, g_env->out_buffer, g_env->full_packet_size, 0, NULL, 0);
	printf("Sent %ld\n", ret);
//	ret = recvmsg(g_env->socket_data.sockfd, (struct msghdr *)g_env->in_buffer, 0);
	ret = read(g_env->socket_data.sockfd, g_env->in_buffer, g_env->full_packet_size);
	if (ret < 0)
		printf("Bad recvmsg\n");
	dump_packet(g_env->out_buffer);
}

static void				init_hints(struct addrinfo *hints)
{
	ft_bzero(hints, sizeof(struct addrinfo));
	if (g_env->flags.ipv6 == true)
		hints->ai_family = AF_INET6;
	else
		hints->ai_family = AF_INET;
	hints->ai_flags = 0;
	hints->ai_protocol = IPPROTO_ICMP;
	hints->ai_socktype = SOCK_RAW;
}

static int32_t			setup_socket()
{
	printf(">>setup_socket called\n");
	char				addrstr[100];
	struct addrinfo		hints;
	struct addrinfo		*ptr;
	struct addrinfo		*start;
	int					ret;

	init_hints(&hints);
	ret = getaddrinfo(g_env->dest, NULL, &hints, &start);
	if (ret != 0)
	{
		printf("ping: %s: Could not resolve hostname\n", g_env->addr_str);
		return (ret);
	}
	for (ptr = start; ptr != NULL; ptr = ptr->ai_next)
	{
		ft_bzero(addrstr, 100);
		inet_ntop(ptr->ai_addr->sa_family, ptr->ai_addr->sa_data, addrstr, 99);
		g_env->socket_data.sockfd = socket(ptr->ai_family, ptr->ai_socktype,
			IPPROTO_ICMP);
		if (g_env->socket_data.sockfd == -1)
		{
			printf("Bad socket() : %s\n", strerror(errno));
			continue;
		}
		if (connect(g_env->socket_data.sockfd, ptr->ai_addr, ptr->ai_addrlen)
			!= -1)
		{
			printf("CONNECTED to %s !\n", addrstr);
			g_env->addr_str = ft_strdup(addrstr);
			g_env->socket_data.addr_dest.sin_addr.s_addr =
				((struct sockaddr_in*)ptr->ai_addr)->sin_addr.s_addr;
			break;
		}
		close(g_env->socket_data.sockfd);
	}
	freeaddrinfo(start);
	return (g_env->socket_data.sockfd);
}

void					run(void)
{
	init_size_values();
	if (getuid() != 0)
	{
		printf("Make sure you're running the program with the "
			"requirered permissions (SU) to open a raw socket\n");
		exit(EXIT_FAILURE);
	}
	if (setup_socket() < 0)
	{
		printf("ping: %s: Name or service unknown\n", g_env->addr_str);
		return ;
	}
	signal(SIGINT, sighandle);
	printf("PING %s (%s) %ld(%ld) bytes of data.\n",
			g_env->dest, g_env->addr_str, g_env->icmp_payload_size,
			g_env->full_packet_size);

	for (uint32_t i = 0; i < g_env->run_data.nb_iter; i++)
	{
		init_icmp_data(g_env->out_buffer + g_env->ip_header_size + g_env->icmp_header_size);
		init_headers();
		exchange();
	}
}
