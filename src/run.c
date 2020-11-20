#include "../inc/ft_ping.h"

/*
static void				debug_addrinfo(struct addrinfo *ptr)
{
	printf("ai_flags = %d\n", ptr->ai_flags);
	printf("ai_family = %d\n", ptr->ai_family);
	printf("ai_socktype = %d\n", ptr->ai_socktype);
	printf("ai_protocol = %d\n", ptr->ai_protocol);
	printf("ai_addrlen = %d\n", ptr->ai_addrlen);
	printf("ai_canonname = %s\n", ptr->ai_canonname);
}

static void				debug_addrinfo_list(struct addrinfo *start)
{
printf(">>debug_addrinfo_list called\n");
	struct addrinfo		*ptr;

	ptr = start;
	while (ptr != NULL)
	{
		debug_addrinfo(ptr);
		ptr = ptr->ai_next;
	}
}
*/

static void				exchange()
{
	ssize_t				ret;

	g_env->icmp_header_size = g_env->flags.ipv6 == true ? sizeof(struct icmp6hdr) : sizeof(struct icmphdr);
	g_env->ip_header_size = g_env->flags.ipv6 == true ? sizeof(struct ip6_hdr) : sizeof(struct iphdr);
	g_env->full_packet_size = g_env->ip_header_size + g_env->icmp_header_size
		+ g_env->icmp_payload_size ;
	init_headers(g_env->out_buffer, g_env->out_buffer + g_env->ip_header_size);
	sendto(g_env->socket_data.sockfd, g_env->out_buffer, g_env->full_packet_size, 0, NULL, 0);
	ret = recvmsg(g_env->socket_data.sockfd, (struct msghdr *)g_env->in_buffer, 0);
	if (ret < 0)
		printf("Bad recvmsg\n");
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
	ret = getaddrinfo(g_env->addr_str, NULL, &hints, &start);
	if (ret != 0)
	{
		printf("ping: %s: Could not resolve hostname\n", g_env->addr_str);
		return (ret);
	}
	for (ptr = start; ptr != NULL; ptr = ptr->ai_next)
	{
		ft_bzero(addrstr, 100);
		inet_ntop(ptr->ai_addr->sa_family, ptr->ai_addr->sa_data, addrstr, 100);
		g_env->socket_data.sockfd = socket(ptr->ai_family, ptr->ai_socktype,
			IPPROTO_ICMP);
		if (g_env->socket_data.sockfd == -1)
		{
			printf("Bad socket() : %s\n", strerror(errno));
			printf("Make sure you're running the program with the "
			"requirered permissions to open a raw socket\n");
			continue;
		}
		if (connect(g_env->socket_data.sockfd, ptr->ai_addr, ptr->ai_addrlen)
			!= -1)
		{
			printf("CONNECTED to %s !\n", addrstr);
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
	if (setup_socket() < 0)
	{
		printf("ping: %s: Name or service unknown\n", g_env->addr_str);
		return ;
	}
	signal(SIGINT, sighandle);
	printf("PING %s (%s) %d(%d) bytes of data.\n",
			g_env->addr_str, g_env->addr_str, 1, 1);

	for (uint32_t i = 0; i < g_env->run_data.nb_iter; i++)
	{
		init_headers();
		exchange();
	}
}
