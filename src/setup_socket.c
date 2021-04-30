#include "../inc/ft_ping.h"

static int32_t			browse_addrlist(struct addrinfo *start)
{
	struct addrinfo		*ptr;
	char				addrstr[100];
	int					optval;
	struct timeval		timeout = {
		.tv_sec = 0,
		.tv_usec = 900000
	};

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
				&optval, sizeof(int)) < 0
			|| setsockopt(g_env->socket_data.sockfd, SOL_SOCKET, SO_RCVTIMEO,
				&timeout, sizeof(struct timeval)) < 0)
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
	int					ret;
	struct addrinfo		*start;
	struct addrinfo		hints = {
		.ai_family = g_env->flags.ipv6 == true ? AF_INET6 : AF_INET,
		.ai_flags = 0,
		.ai_protocol = IPPROTO_ICMP,
		.ai_socktype = SOCK_RAW
	};

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
