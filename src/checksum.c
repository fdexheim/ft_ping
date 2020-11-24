#include "../inc/ft_ping.h"

static void			flip_bits(uint16_t *target)
{
	uint16_t		mask;

	printf("before flip target = %d\n", *target);
	mask = 0xffff;
	*target ^= mask;
	printf("after flip target = %d\n", *target);
}

uint16_t			calculate_checksum(void *hdr, uint32_t iters)
{
	uint16_t		*ptr;
	uint16_t		ret;

	ptr = hdr;
	ret = 0;
	for (uint32_t i = 0; i < iters; i++)
	{
		ret += *ptr;
		ptr++;
	}
	flip_bits(&ret);
	return (ret);
}
