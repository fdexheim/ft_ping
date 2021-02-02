
#include "../inc/ft_ping.h"

static bool			parse_no_arg_flag()
{
	uint32_t		j;
	bool			was_no_arg_flag;

	j = 1;
	was_no_arg_flag = false;
	while (j < ft_strlen(g_env->argv[g_env->i]))
	{
		switch (g_env->argv[g_env->i][j])
		{
			case 'h':
			{
				g_env->flags.h = true;
				was_no_arg_flag = true;
				break;
			}
			case 'V':
			{
				g_env->flags.V = true;
				was_no_arg_flag = true;
				break;
			}
			case 'v':
			{
				g_env->flags.v = true;
				g_env->flags.verbose_level++;
				was_no_arg_flag = true;
				break;
			}
			case '6':
			{
				g_env->flags.ipv6 = true;
				was_no_arg_flag = true;
				break;
			}
			default:
				break;
		}
		j++;
	}
	return (was_no_arg_flag);
}

static void			handle_flag_t()
{
	uint32_t		tmp;

	if (g_env->i + 1 >= g_env->argc)
	{
		printf("Missing argument for -t option\n");
		usage();
		exit(EXIT_SUCCESS);
	}
	g_env->flags.t = true;
	tmp = ft_atoi(g_env->argv[g_env->i + 1]);
	if (tmp > 255)
		g_env->run_data.forced_ttl = 255;
	else
		g_env->run_data.forced_ttl = tmp;
}

static void			handle_flag_c()
{
	int32_t			tmp;

	if (g_env->i + 1 >= g_env->argc)
	{
		printf("Missing argument for -t option\n");
		usage();
		exit(EXIT_SUCCESS);
	}
	g_env->flags.c = true;
	tmp = ft_atoi(g_env->argv[g_env->i + 1]);
	if (tmp <= 0)
	{
		printf("ft_ping: bad number of packets to transmit\n");
		exit(EXIT_SUCCESS);
	}
	else
		g_env->run_data.nb_iter = tmp;
}

static void			parse_arg_flag()
{
	uint32_t		nb_tokens;

	nb_tokens = 0;
	switch (g_env->argv[g_env->i][1])
	{
		case 't':
			handle_flag_t();
			nb_tokens = 1;
			break;
		case 'c':
			handle_flag_c();
			nb_tokens = 1;
			break;
		default:
			nb_tokens = 0;
			break;
	}
	g_env->i += nb_tokens;
}

static void			parse_flag()
{
	if (parse_no_arg_flag() == false)
	{
		parse_arg_flag();
	}
}

void				parse(void)
{
	g_env->i = 1;
	while (g_env->i < g_env->argc)
	{
		if (g_env->argv[g_env->i][0] == '-'
			&& ft_strlen(g_env->argv[g_env->i]) > 1)
		{
			parse_flag();
		}
		else if (g_env->dest == NULL)
			g_env->dest = g_env->argv[g_env->i];
		g_env->i++;
	}
}
