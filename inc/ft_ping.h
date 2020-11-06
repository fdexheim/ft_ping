#ifndef FT_PING_H
# define FT_PING_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct		s_flags
{
	bool			v;
	bool			V;
	bool			h;
	bool			ipv6;
}					t_flags;

typedef struct		s_env
{
	uint32_t		i;
	int				argc;
	char			**argv;
	char			*addr;
	t_flags			flags;
}					t_env;

t_env			*g_env;

void			usage();
void			parse();

#endif
