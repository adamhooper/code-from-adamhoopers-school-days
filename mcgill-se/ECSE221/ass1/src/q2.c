#include <stdio.h>
#include <limits.h>

#define is_positive(i) (((i) & 0x80000000) ? 0 : 1)
#define array_num_elements(arr) (sizeof (arr) / sizeof (arr[0]))

typedef struct
{
	long multiplier;
	long multiplicand;
	long product;
	int result;
} Test;

static Test tests[] =
{
	{ 1L,		1L,		1L,		0	},
	{ 46340L,	46341L,		2147441940L,	0	},
	{ 46341L,	46341L,		0L,		-1	},
	{ 0L,		LONG_MAX,	0L,		0	},
	{ 1L,		LONG_MAX,	LONG_MAX,	0	},
	{ 2L,		LONG_MAX,	0L,		-1	},
	{ -1L,		LONG_MAX,	-2147483647L,	0	},
	{ 0L,		0L,		0L,		0	},
	{ 46340L,	-46341L,	-2147441940L,	0	},
	{ LONG_MAX,	-1L,		-2147483647L,	0,	},
};
static const int num_tests = array_num_elements (tests);

int
product (long multiplier,
	 long multiplicand,
	 long *product)
{
	char is_negative = 0; /* 0: positive, 1: negative */
	*product = 0;

	/* Deal with the sign -- make multiplier & multiplicand positive */
	if (is_positive (multiplier) != is_positive (multiplicand))
	{
		is_negative = 1;
	}
	if (!is_positive (multiplier))
	{
		multiplier = -multiplier;
	}
	if (!is_positive (multiplicand))
	{
		multiplicand = -multiplicand;
	}

	/* Perform the multiplication (on positive numbers) */
	while (multiplier != 0)
	{
		/* printf ("a: %ld, b: %ld\n", multiplier, multiplicand); */

		/* Has multiplicand overflowed? */
		if (!is_positive (multiplicand)) return -1;

		if (multiplier & 1)
		{
			/* Will this addition overflow? */
			if (LONG_MAX - *product < multiplicand) return -1;

			*product += multiplicand;
		}


		multiplier >>= 1;
		multiplicand <<= 1;
	}

	if (is_negative)
	{
		*product = -*product;
	}

	return 0;
}

void
run_test (long multiplier,
	  long multiplicand,
	  long expected_product,
	  int expected_result)
{
	long test_product;
	int test_result;

	test_result = product (multiplier, multiplicand, &test_product);

	if (test_result != expected_result)
	{
		printf ("FAILURE on %ld * %ld\n",
			multiplier, multiplicand);

		return;
	}

	/* Check that test_product is valid */
	/* By now, we know test_result is */

	if (test_result == -1 || test_product == expected_product)
	{
		printf ("Success on %ld * %ld\n",
			multiplier, multiplicand);
	}
	else
	{
		printf ("FAILURE on %ld * %ld\n",
			multiplier, multiplicand);
	}
}

int
main (int argc,
      char **argv)
{
	unsigned int i;

	for (i = 0; i < num_tests; i++)
	{
		run_test (tests[i].multiplier, tests[i].multiplicand,
			  tests[i].product, tests[i].result);
	}

	return 0;
}
