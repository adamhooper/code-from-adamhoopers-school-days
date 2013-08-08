#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * names.c: sorts the given arguments in ascending order and writes them to
 *          a file.
 *
 * usage: ./names.c [name1] [name2] [name3] ...
 */

#define	FILENAME	"names.txt"
#define	BUFFER_SIZE	1024

/* Removes trailing escape characters from s (i.e., newlines) */
static void
strchomp (char *s)
{
	while (*s > 20)
	{
		s++;
	}

	*s = '\0';
}

static void
insert_str_in_file (const char *str)
{
	FILE *fpr;
	FILE *fpw;
	char buffer[BUFFER_SIZE];
	int printed = 0;

	fpr = fopen (FILENAME, "r");

	/* If file doesn't exist, write a new one and return */
	if (fpr == NULL)
	{
		fpw = fopen (FILENAME, "w");
		if (fpw == NULL)
		{
			perror ("Could not open for write");
			return;
		}
		fprintf (fpw, "%s\n", str);
		fclose (fpw);
		return;
	}

	/* unlink the existing file (we can still read from it until fclose) */
	remove (FILENAME);

	fpw = fopen (FILENAME ".new", "w");
	if (fpw == NULL)
	{
		perror ("Could not open for write");
		fclose (fpr);
		return;
	}

	/* copy input to output, inserting str if it's in the right order */
	while (fgets (buffer, BUFFER_SIZE, fpr) != NULL)
	{
		strchomp (buffer);
		if (printed != 1 && strcmp (buffer, str) > 0)
		{
			fprintf (fpw, "%s\n", str);
			printed = 1;
		}
		fprintf (fpw, "%s\n", buffer);
	}

	if (printed != 1)
	{
		fprintf (fpw, "%s\n", str);
	}

	fclose (fpr);
	fclose (fpw);

	rename (FILENAME ".new", FILENAME);
}

int
main (int argc, char **argv)
{
	if (argc != 2)
	{
		printf ("Usage: %s [name]\n", argv[0]);
		return 1;
	}

	insert_str_in_file (argv[1]);

	return 0;
}
