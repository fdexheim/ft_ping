#include "../inc/ft_ping.h"

void			usage(void)
{
	printf("Usage: ./ft_ping [dest addr]\n");
	printf("flags :\n");
	printf("-c [count] | Run up to [count] iterations\n");
	printf("-h         | Help pannel (what you're reading right now)\n");
	printf("-t [ttl]   | Set specific ttl value in ip header\n");
	printf("-v         | Verbose mode. add up to 2 v's for more verbose\n");
	printf("-6         | ipv6 mode. NYI\n");
	return ;
}

void			close_env()
{
	if (g_env != NULL)
	{
		close(g_env->socket_data.sockfd);
		if (g_env->addr_str != NULL)
			free(g_env->addr_str);
		free(g_env);
		g_env = NULL;
	}
}

static void		setup_env(int argc, char **argv)
{
	if ((g_env = malloc(sizeof(t_env))) == NULL)
	{
		printf("[ERROR] Failed memory allocation\n");
		exit(EXIT_FAILURE);
	}
	ft_bzero(g_env, sizeof(t_env));
	g_env->argc = argc;
	g_env->argv = argv;
	return ;
}

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

int				main(int argc, char **argv)
{
	if (argc <= 1)
	{
		usage();
		close_env();
		exit(EXIT_SUCCESS);
	}
	setup_env(argc, argv);
	parse();
	if (g_env->dest == NULL || g_env->flags.h == true)
	{
		usage();
		close_env();
		exit(EXIT_SUCCESS);
	}
	init_size_values();
	run();
	close_env();
}
