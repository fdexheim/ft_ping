#include "../inc/ft_ping.h"

static void					println_status(suseconds_t diff_usec)
{
	printf("icmp_seq=%d ttl=%d ", g_env->run_data.current_iter, 115);
	printf("time=%ld.%ld ms\n", diff_usec / 1000, (diff_usec % 1000) / 100);
}


void					record_statistics_success(suseconds_t rtt)
{
	if (g_env->run_data.recorded_iters == 0)
	{
		g_env->run_data.min = rtt;
		g_env->run_data.avg = rtt;
		g_env->run_data.max = rtt;
		g_env->run_data.sum = rtt;
		g_env->run_data.mdev = 0;
	}
	else
	{
		if (rtt < g_env->run_data.min)
			g_env->run_data.min = rtt;
		if (rtt > g_env->run_data.max)
			g_env->run_data.max = rtt;
		g_env->run_data.sum += rtt;
	}
	g_env->run_data.recorded_iters++;
	println_status(rtt);
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
