#include "../inc/ft_ping.h"

void			usage(void)
{
	printf("Usage: ");
	return ;
}

static void		close_env()
{
	if (g_env != NULL)
	{
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
		printf("Failed memory allocation\n");
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

static void		init_default_values()
{
	if (g_env->run_data.nb_iter == 0)
		g_env->run_data.nb_iter = 10;
}

int				main(int argc, char **argv)
{
	if (argc <= 1)
	{
		usage();
		exit(EXIT_SUCCESS);
	}
	setup_env(argc, argv);
	parse();
	init_default_values();
	init_size_values();
	run();
	close_env();
}
