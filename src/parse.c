
#include "../inc/ft_ping.h"

static bool		parse_no_arg_flag()
{
	int			j;
	bool		was_no_arg_flag;

	j = 1;
	was_no_arg_flag = false;
	while (j < ft_strlen(g_env->argv[g_env->i]))
	{
		switch (g_env->argv[g_env->i][j])
		{
			case 'V':
			{
				g_env->flags.V = true;
				was_no_arg_flag = true;
				break;
			}
			case 'v':
			{
				g_env->flags.v = true;
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

static void		handle_flag_h()
{
	
}

static void		parse_arg_flag()
{
	bool		valid_option;

	valid_option = true;
	if (g_env->i == g_env->argc)
	{
		printf("ft_ping: option requires an argument -- %c",
			g_env->argv[g_env->i][1]);
		usage();
		exit(EXIT_FAILURE);
	}
	switch (g_env->argv[g_env->i][1])
	{
		case 'h':
			handle_flag_h();
			break;
		default:
			valid_option = false;
			break;
	}
	if (valid_option == true)
		g_env->i++;
}

static void		parse_flag()
{
	printf("parse flag");
	if (parse_no_arg_flag == false)
	{
		parse_arg_flag();
	}
}

void			parse(void)
{
	g_env->i = 1;
	while (g_env->i < g_env->argc)
	{
		if (g_env->argv[g_env->i][0] == '-'
			&& ft_strlen(g_env->argv[g_env->i]) > 1)
		{
			parse_flag();
		}
		else if (g_env->addr == NULL)
			g_env->addr = g_env->argv[g_env->i];
		g_env->i++;
	}
}
