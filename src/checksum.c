#include "../inc/ft_ping.h"

static void			flip_bits(uint16_t *target)
{
	uint16_t		mask;

	printf("before flip target = %x\n", *target);
	mask = 0xffff;
	*target ^= mask;
	printf("after flip target = %x\n", *target);
}

uint16_t			calculate_checksum(void *hdr, uint32_t iters)
{
	uint16_t		*ptr;
	uint32_t		tot;
	uint32_t		carry;
	uint16_t		ret;

	ptr = hdr;
	tot = 0;
	carry = 0;
	ret = 0;
	for (uint32_t i = 0; i < iters; i++)
	{
		tot += *ptr;
		ptr++;
	}
	carry = (tot & 0xffff0000) >> 16;
	ret = (tot & 0x0000ffff) + carry;
	printf("tot = %x | carry = %x | ret = %x\n", tot, carry, ret);
	flip_bits(&ret);
	printf("tot = %x | carry = %x | ret = %x\n", tot, carry, ret);
	return (ret);
}
