#include "../inc/ft_ping.h"

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
		inet_ntop(ptr->ai_addr->sa_family, ptr->ai_addr->sa_data + 2, addrstr, 99);
		g_env->socket_data.sockfd = socket(AF_INET, SOCK_RAW,
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
		g_env->addr_str = ft_strdup(addrstr);
		g_env->socket_data.addr_dest.sin_addr.s_addr =
			((struct sockaddr_in*)ptr->ai_addr)->sin_addr.s_addr;
		g_env->socket_data.addr_dest.sin_family = AF_INET;
		break;
	}
	return (g_env->socket_data.sockfd);
}

int32_t					setup_socket()
{
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
	browse_addrlist(start);
	if (start != NULL)
		freeaddrinfo(start);
	return (g_env->socket_data.sockfd);
}
