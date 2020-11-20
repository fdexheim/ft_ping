#ifndef FT_PING_H
# define FT_PING_H

#include <arpa/inet.h>
#include <errno.h>
#include <linux/icmp.h>
#include <linux/icmpv6.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "../libft/inc/libft.h"

#define PACKET_MAX_SIZE 4096
#define ICMP_PAYLOAD_SIZE 56

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
	struct sockaddr_in		addr_dest;
	struct sockaddr_in6		addrv6_dest;
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
	unsigned char			in_buffer[PACKET_MAX_SIZE];
	unsigned char			out_buffer[PACKET_MAX_SIZE];
	size_t					ip_header_size;
	size_t					icmp_header_size;
	size_t					icmp_payload_size;
	size_t					full_packet_size;
}							t_env;

t_env						*g_env;

void						init_headers();
void						init_icmp_header(void *header_start);
void						init_ip_header(void *header_start);
void						run();
void						usage();
void						parse();
void						sighandle(int sig);

#endif
