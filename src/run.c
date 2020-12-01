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

static int32_t			browse_addrlist(struct addrinfo *start)
{
	struct addrinfo		*ptr;
	char				addrstr[100];
	int					optval;

	optval = 1;
	for (ptr = start; ptr != NULL; ptr = ptr->ai_next)
	{
		ft_bzero(addrstr, 100);
		inet_ntop(ptr->ai_addr->sa_family, ptr->ai_addr->sa_data, addrstr, 99);
		g_env->socket_data.sockfd = socket(ptr->ai_family, ptr->ai_socktype,
			IPPROTO_ICMP);
		if (g_env->socket_data.sockfd == -1)
		{
			printf("Bad socket()\n");
			continue;
		}
		if (setsockopt(g_env->socket_data.sockfd, IPPROTO_IP, IP_HDRINCL,
			&optval, sizeof(int)) < 0)
		{
			printf("Bad setsockopt()\n");
			exit(EXIT_FAILURE);
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
	return (g_env->socket_data.sockfd);
}

static int32_t			setup_socket()
{
	printf(">>setup_socket called\n");
	struct addrinfo		hints;
	struct addrinfo		*start;
	int					ret;

	init_hints(&hints);
	ret = getaddrinfo(g_env->dest, NULL, &hints, &start);
	if (ret != 0)
	{
		printf("ping: %s: Could not resolve hostname\n", g_env->addr_str);
		return (ret);
	}
	dump_addrinfo_list(start);
	browse_addrlist(start);
	if (start != NULL)
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
		ft_bzero(g_env->out_buffer, 4096);
		ft_bzero(g_env->in_buffer, 4096);
		init_icmp_data(g_env->out_buffer + g_env->ip_header_size + g_env->icmp_header_size);
		init_headers();
		exchange();
	}
	sighandle(SIGINT);
}
