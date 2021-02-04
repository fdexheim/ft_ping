#include "../inc/ft_ping.h"

void				recap()
{
	float			packet_ratio;
	time_t			span_sec;
	suseconds_t		total_span;
	uint64_t		avg;

	if (g_env->run_data.recorded_sucessful_iters == 0)
		avg = 0;
	else
		avg = g_env->run_data.sum / g_env->run_data.recorded_sucessful_iters;

	span_sec = g_env->run_data.time_end.tv_sec - g_env->run_data.time_start.tv_sec;
	total_span = (span_sec * 1000000 + g_env->run_data.time_end.tv_usec - g_env->run_data.time_start.tv_usec) / 1000;
	packet_ratio = 100 - 100 * ((float)(g_env->run_data.nb_packets_received) / (float)(g_env->run_data.nb_packets_sent));

	printf("\n--- %s (%s) ping statistics ---\n", g_env->dest, g_env->addr_str);
	printf("%d packets transmitted, %d received, ",
		g_env->run_data.nb_packets_sent,
		g_env->run_data.nb_packets_received);
	if (g_env->run_data.nb_packets_errors > 0)
		printf("+%d errors, ", g_env->run_data.nb_packets_errors);
	printf("%.0f%% packet loss, time %ldms\n", packet_ratio, total_span);
	if (g_env->run_data.recorded_sucessful_iters > 0)
	{
		printf("rtt min/avg/max/mdec = %ld.%ld/%ld.%ld/%ld.%ld/%d ms",
			g_env->run_data.min / 1000, g_env->run_data.min % 1000,
			avg / 1000, avg % 1000,
			g_env->run_data.max / 1000, g_env->run_data.max % 1000,
			42);
	}
	printf("\n");
}

void			sighandle(int sig)
{
	(void)sig;
	recap();
	close_env();
	exit(EXIT_SUCCESS);
}
