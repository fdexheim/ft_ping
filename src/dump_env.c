#include "../inc/ft_ping.h"

void					dump_flags(t_flags *flags)
{
	printf(" === FLAGS DUMP === ");
	printf("flag v = %d\n", flags->v);
	printf("flag V = %d\n", flags->V);
	printf("flag h = %d\n", flags->h);
	printf("flag ipv6 = %d\n", flags->ipv6);
}

void					dump_env(t_env *env)
{
	printf(" === ENV DUMP === \n");
	dump_flags(&env->flags);
}
