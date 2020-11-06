#include "../inc/ft_ping.h"

static void		usage(void)
{
	printf("Usage: ");
	return ;
}

static void		setup_env(int argc, char **argv)
{
	if ((g_env = malloc(sizeof(t_env))) == NULL)
	{
		printf("Failed memory allocation\n");
		exit(EXIT_FAILURE);
	}
	g_env->argc = argc;
	g_env->argv = argv;
	bzero(g_env, sizeof(t_env));
	return ;
}

int				main (int argc, char **argv)
{
	if (argc <= 1)
	{
		usage();
		exit(EXIT_SUCCESS);
	}
	setup_env(argc, argv);
	parse();
}
