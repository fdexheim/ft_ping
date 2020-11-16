#include "../inc/ft_ping.h"

static void				exchange()
{
//	usleep(1);
}

static int32_t			setup_socket(char *addr)
{
printf(">>set_addr called\n"); 
	int					ret;

	if (g_env->flags.ipv6 == true)
		ret = inet_pton(AF_INET6, addr, &g_env->socket_data.addrv6);
	else
		ret = inet_pton(AF_INET, addr, &g_env->socket_data.addr);
	if (ret <= 0)
	{
		if (ret == 0)
			return (0);
		else
			printf("invalid af parameter (somehow)\n");
		exit(EXIT_FAILURE);
	}
	//if (env->flags.ipv6 == true)
	//	g_env->socket_data.sockfd = socket()
	//
	return (1);
}

static void				debug_addrinfo(struct addrinfo *ptr)
{
	printf("ai_flags = %d\n", ptr->ai_flags);
	printf("ai_family = %d\n", ptr->ai_family);
	printf("ai_socktype = %d\n", ptr->ai_socktype);
	printf("ai_protocol = %d\n", ptr->ai_protocol);
	printf("ai_addrlen = %d\n", ptr->ai_addrlen);
	printf("ai_canonname = %s\n", ptr->ai_canonname);
//	printf("ai_ = %\n", ptr->ai_);
}

static int32_t			setup_socket_from_getaddrinfo()
{
	char				addrstr[100];

printf(">>set_addr_from_info called\n"); 
	struct addrinfo		hints;
	struct addrinfo		*ptr;
	struct addrinfo		*start;
	int					ret;

	ft_bzero(&hints, sizeof(struct addrinfo));
	if (g_env->flags.ipv6 == true)
		hints.ai_family = AF_INET6;
	else
		hints.ai_family = AF_INET;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;
	if (g_env->flags.ipv6 == true)
	hints.ai_socktype = SOCK_RAW;

	ret = getaddrinfo(g_env->addr_str, NULL, &hints, &start);

	if (ret != 0)
	{
		printf("ping: %s: Could not resolve hostname\n", g_env->addr_str);
		return (ret);
	}

	for (ptr = start; ptr != NULL; ptr = ptr->ai_next)
	{
		debug_addrinfo(ptr);
//		if (1)
//		{
			ft_bzero(addrstr, 100);
			inet_ntop(ptr->ai_family, ptr->ai_addr->sa_data, addrstr, 100);
			g_env->socket_data.sockfd = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			printf("addrstr = %s, sockfd = %d\n", addrstr, g_env->socket_data.sockfd);
			if (g_env->socket_data.sockfd == -1)
				continue;
//			if (connect(g_env->socket_data.sockfd, ptr->ai_addr, ptr->ai_addrlen) != -1)
//			{
//				printf("CONNECTED to %s !\n", addrstr);
//				break;
//			}
			close(g_env->socket_data.sockfd);
//		}
	}

	freeaddrinfo(start);

	return (1);
}

static int32_t			setup_connection()
{
printf(">>setup_connection called\n"); 
	int32_t				ret;

	ret = setup_socket(g_env->addr_str);
	if (ret <= 0)
		ret = setup_socket_from_getaddrinfo();
	if (ret < 0)
		return -1;
	return 1;
}

void					run(void)
{
	struct timeval		start_iter;
	struct timeval		end_iter;
//	struct timeval		diff;

	if (setup_connection() < 0)
	{
		printf("ping: %s: Name or service unknown\n", g_env->addr_str);
		return ;
	}
	signal(SIGINT, sighandle);
	printf("PING %s (%s) %d(%d) bytes of data.\n",
		g_env->addr_str, g_env->addr_str, 1, 1);
	for (uint32_t i = 0; i < g_env->run_data.nb_iter; i++)
	{
//		printf("loop iter %d\n", i + 1);
		gettimeofday(&start_iter, NULL);
		exchange();
		gettimeofday(&end_iter, NULL);
//		diff.tv_sec = end_iter.tv_sec - start_iter.tv_sec;
//		diff.tv_usec = end_iter.tv_usec - start_iter.tv_usec;
//		printf("time start = %ld.%ld | time end = %ld.%ld | diff = %ld.%ld\n",
//			start_iter.tv_sec, start_iter.tv_usec,
//			end_iter.tv_sec, end_iter.tv_usec,
//			diff.tv_sec, diff.tv_usec);
	}
}
