#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Evaluates a and b more than once */
#define max(a,b) ((a) > (b) ? (a) : (b))

char *
my_add (char *i1,
	char *i2)
{
	char *ret, *ret_p;
	char *i1_p = i1;
	char *i2_p = i2;
	char len;
	char carry = 0;
	char sum;

	while (*i1_p) ++i1_p;
	while (*i2_p) ++i2_p;

	len = 1 + max (i1_p - i1, i2_p - i2); /* 1 for potential carry */

	ret = calloc (len + 1, sizeof (char)); /* 1 for NULL */

	ret_p = ret + len;

	while (--ret_p >= ret)
	{
		sum = 0;

		if (carry)
		{
			sum += carry;
			carry = 0;
		}

		if (--i1_p >= i1)
		{
			sum += *i1_p - '0';
		}

		if (--i2_p >= i2)
		{
			sum += *i2_p - '0';
		}

		if (sum > 9)
		{
			carry = 1;
			sum -= 10;
		}

		*ret_p = sum + '0';
	}

	return ret;
}

int
main (int argc,
      char **argv)
{
	char *sum;
	char *p;

	if (argc != 3)
	{
		fprintf (stderr, "usage: %s [number] [number]\n", argv[0]);
		return 1;
	}

	p = sum = my_add(argv[1], argv[2]);

	while (*p == '0')
	{
		p++;
	}

	printf ("%s + %s = %s\n", argv[1], argv[2], p);

	free (sum);

	return 0;
}
