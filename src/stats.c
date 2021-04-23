#include "../inc/ft_ping.h"

void					record_statistics_success(suseconds_t rtt)
{

	if (g_env->run_data.recorded_sucessful_iters == 0)
	{
		g_env->run_data.min = rtt;
		g_env->run_data.max = rtt;
	}
	else
	{
		if (rtt < g_env->run_data.min)
			g_env->run_data.min = rtt;
		if (rtt > g_env->run_data.max)
			g_env->run_data.max = rtt;
	}
	g_env->run_data.sum += rtt;
	g_env->run_data.sum2 += (rtt * rtt);
	g_env->run_data.recorded_sucessful_iters++;
}

suseconds_t				get_rtt_sus(struct timeval *start,
struct timeval *end)
{
	time_t				diff_sec;
	suseconds_t			diff_usec;

	diff_sec = end->tv_sec - start->tv_sec;
	diff_usec = 1000000 * diff_sec + (end->tv_usec - start->tv_usec);

	return (diff_usec);
}
