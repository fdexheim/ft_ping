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

int				main (int argc, char **argv)
{
	if (argc <= 1)
	{
		usage();
		exit(EXIT_SUCCESS);
	}
	setup_env(argc, argv);
	parse();
	run();
	close_env();
}
