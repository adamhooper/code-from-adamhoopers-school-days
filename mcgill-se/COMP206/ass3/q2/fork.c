#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define FILENAME "number.txt"

static int
read_num_from_file (const char *filename)
{
	FILE *fp;
	char buffer[BUFFER_SIZE];
	int num;

	fp = fopen (filename, "r");
	if (fp == NULL)
	{
		perror ("fopen");
		return -1;
	}

	fgets (buffer, BUFFER_SIZE, fp);

	fclose (fp);

	num = atoi (buffer);

	return num;
}

static void
write_num_to_file (int num, const char *filename)
{
	FILE *fp;

	fp = fopen (filename, "w");
	if (fp == NULL)
	{
		perror ("fopen");
		return;
	}

	if (fprintf (fp, "%d\n", num) == 0)
	{
		perror ("Couldn't print number to file");
	}

	fclose (fp);
}

static int
file_exists (const char *filename)
{
	struct stat buf;
	return (stat (filename, &buf) == 0);
}

static void
run_parent (void)
{
	int i;
	int num;

	for (i = 0; i < 10; i++)
	{
		while (file_exists (FILENAME))
			;

		num = rand ();

		printf ("Parent generates: %d\n", num);
		write_num_to_file (num, FILENAME);
	}

	printf ("Parent terminates\n");
}

static void
run_child (void)
{
	int i;
	int num;

	for (i = 0; i < 10; i++)
	{
		while (!file_exists (FILENAME))
			;

		num = read_num_from_file (FILENAME);

		printf ("Child reads: %d\n", num);

		if (unlink (FILENAME) == -1)
		{
			perror ("unlink");
		}
	}

	printf ("Child terminates\n");
}

int
main (int argc, char **argv)
{
    pid_t pid;

	srand (time (NULL));

	unlink (FILENAME);

	pid = fork ();
	if (pid != 0)
	{
		run_parent ();
        waitpid (pid, NULL, 0);
	}
	else
	{
		run_child ();
	}

	return 0;
}
