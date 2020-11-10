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

static void		prepare_default_values()
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
	prepare_default_values();
	parse();
	run();
	close_env();
}
