#include <stdio.h>
#include <string.h>

typedef struct
{
	const char i[5];
	const char o[4];
} lookup_field;

static lookup_field table[] = {
	{ "0000", "000" },
	{ "0001", "010" },
	{ "0010", "010" },
	{ "0011", "000" },
	{ "0100", "001" },
	{ "0101", "000" },
	{ "0110", "001" },
	{ "0111", "000" },
	{ "1000", "101" },
	{ "1001", "000" },
	{ "1010", "001" },
	{ "1011", "000" },
	{ "1100", "100" },
	{ "1101", "110" },
	{ "1110", "110" },
	{ "1111", "000" }
};

void StateMachine (char M, char *Q2, char *Q1, char *Q0)
{
	int i = 0;
	char lookup[5];

	snprintf (lookup, 5, "%d%d%d%d", M, *Q2, *Q1, *Q0);

	for (i = 0; ; i++) /* Input had better be valid! */
	{
		if (strcmp (lookup, table[i].i) == 0)
		{
			*Q2 = table[i].o[0] - '0';
			*Q1 = table[i].o[1] - '0';
			*Q0 = table[i].o[2] - '0';
			return;
		}
	}
}

void print_table (void)
{
	int i;

	for (i = 0; i < 16; i++)
	{
		printf ("%s maps to %s\n", table[i].i, table[i].o);
	}
}

int main (int argc, char **argv)
{
	char M, i_Q2, i_Q1, i_Q0, o_Q2, o_Q1, o_Q0;

	if (argc != 2 || strlen (argv[1]) != 4)
	{
		fprintf (stderr, "usage: %s [4-digit state]\n",
			 argv[0]);
		return 1;
	}

	M = argv[1][0]    != '0';
	i_Q2 = argv[1][1] != '0';
	i_Q1 = argv[1][2] != '0';
	i_Q0 = argv[1][3] != '0';

	o_Q2 = i_Q2;
	o_Q1 = i_Q1;
	o_Q0 = i_Q0;

	StateMachine (M, &o_Q2, &o_Q1, &o_Q0);

	printf ("Input of %d%d%d%d yeilds %d%d%d\n",
		M, i_Q2, i_Q1, i_Q0, o_Q2, o_Q1, o_Q0);

	return 0;
}
