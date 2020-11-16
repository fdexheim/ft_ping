
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
/*
static void			handle_flag_h()
{
	
}
*/
static void			parse_arg_flag()
{
	bool			valid_option;

	valid_option = true;
	switch (g_env->argv[g_env->i][1])
	{
		default:
			valid_option = false;
			break;
	}
	if (valid_option == true)
		g_env->i++;
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
printf("Parse loop start, argc = %d\n", g_env->argc);

	g_env->i = 1;
	while (g_env->i < g_env->argc)
	{
		printf("arg : %s\n", g_env->argv[g_env->i]);
		if (g_env->argv[g_env->i][0] == '-'
			&& ft_strlen(g_env->argv[g_env->i]) > 1)
		{
			parse_flag();
		}
		else if (g_env->addr_str == NULL)
			g_env->addr_str = g_env->argv[g_env->i];
		g_env->i++;
	}
	printf("End of Parse, addr = %s\n", g_env->addr_str);
	printf("flag v = %d\n", g_env->flags.v);
	printf("flag V = %d\n", g_env->flags.V);
	printf("flag h = %d\n", g_env->flags.h);
	printf("flag ipv6 = %d\n", g_env->flags.ipv6);
}
