#ifndef FT_PING_H
# define FT_PING_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "../libft/inc/libft.h"

typedef struct				s_flags
{
	bool					v;
	bool					V;
	bool					h;
	bool					ipv6;
}							t_flags;

typedef struct				s_run_data
{
	uint32_t				nb_iter;
	uint32_t				nb_packets_sent;
	uint32_t				nb_packets_reveived;
	time_t					delay;
	time_t					total_run_time;
}							t_run_data;

typedef struct				s_socket_data
{
	int						sockfd;
	struct in_addr			addr;
	struct in6_addr			addrv6;
}							t_socket_data;

typedef struct				s_env
{
	int						i;
	int						argc;
	char					**argv;
	char					*addr_str;
	t_flags					flags;
	t_run_data				run_data;
	t_socket_data			socket_data;
}							t_env;

t_env						*g_env;

void						run();
void						usage();
void						parse();
void						sighandle(int sig);

#endif
