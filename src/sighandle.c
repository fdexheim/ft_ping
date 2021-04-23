#include "../inc/ft_ping.h"

// sqrt not in authorised functions so this will have to do
// the result will not be exact on the fractionnal part but it is close enough
static suseconds_t	bootleg_sqrt(suseconds_t src)
{
	suseconds_t		ret;

	ret = 1;
	while ((ret + 1) * (ret + 1) <= src)
	{
		ret += 1;
	}
	return (ret);
}

static void			print_packets_info()
{
	float			packet_ratio;
	time_t			span_sec;
	suseconds_t		total_span;

	span_sec = g_env->run_data.time_end.tv_sec - g_env->run_data.time_start.tv_sec;
	total_span = (span_sec * 1000000 + g_env->run_data.time_end.tv_usec - g_env->run_data.time_start.tv_usec) / 1000;
	packet_ratio = 100 - 100 * ((float)(g_env->run_data.nb_packets_received) / (float)(g_env->run_data.nb_packets_sent));

	printf("\n--- %s ping statistics ---\n", g_env->dest);
	printf("%d packets transmitted, %d received, ",
		g_env->run_data.nb_packets_sent,
		g_env->run_data.nb_packets_received);
	if (g_env->run_data.nb_packets_errors > 0)
		printf("+%d errors, ", g_env->run_data.nb_packets_errors);
	printf("%.0f%% packet loss, time %ldms\n", packet_ratio, total_span);
}

void				recap()
{
	suseconds_t		avg;
	suseconds_t		mdev;

	print_packets_info();
	if (g_env->run_data.recorded_sucessful_iters > 0)
	{
		avg = g_env->run_data.sum / g_env->run_data.recorded_sucessful_iters;
		mdev = bootleg_sqrt((g_env->run_data.sum2 / g_env->run_data.recorded_sucessful_iters) - (avg * avg));

		printf("rtt min/avg/max/mdec = %ld.%03ld/%ld.%03ld/%ld.%03ld/%ld.%03ld ms\n",
			g_env->run_data.min / 1000, g_env->run_data.min % 1000,
			avg / 1000, avg % 1000,
			g_env->run_data.max / 1000, g_env->run_data.max % 1000,
			mdev / 1000, mdev % 1000);
	}
}

void			sighandle(int sig)
{
	(void)sig;
	recap();
	close_env();
	exit(EXIT_SUCCESS);
}
