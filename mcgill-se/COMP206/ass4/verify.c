#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTH_FILENAME "users.txt"
#define INPUT_BUFFER_SIZE 1024

static int
authenticate (const char *username,
	      const char *password)
{
	FILE *f;
	char input[INPUT_BUFFER_SIZE];

	char *end_username;
	char *end_password;

	int username_matches;
	int password_matches;

	f = fopen (AUTH_FILENAME, "r");
	if (f == NULL) return 0;

	while (fgets (input, INPUT_BUFFER_SIZE, f) != NULL)
	{
		end_username = strchr (input, ' ');
		if (end_username == NULL)
		{
			fclose (f);
			return 0;
		}

		end_password = strstr (input, "\n");
		if (end_password == NULL)
		{
			fclose (f);
			return 0;
		}

		username_matches = (strncmp (input, username,
					     end_username - input) == 0);
		password_matches = (strncmp (end_username + 1, password,
					     end_password - end_username - 1)
				    == 0);

		if (username_matches && password_matches)
		{
			break;
		}
	}

	fclose (f);

	return username_matches && password_matches;
}

static void
print_header (void)
{
	/* XXX: Wrong content-type but IE balks on proper XHTML one */
	printf ("Content-type: text/html; charset=utf-8\n\n");

	printf ("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHMTL 1.1//EN\"\n"
		"          \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n"
		"<html xmlns=\"http://www.w3.org/1999/xhtml\""
			" xml:lang=\"en\">\n"
		"<head>\n"
		" <title>Log in</title>\n"
		" <link rel=\"stylesheet\" type=\"text/css\""
			" href=\"style.css\"/>\n"
		" <meta name=\"Author\" content=\"Adam Hooper\"/>\n"
		"</head>\n"
		"<body>\n");
}

static void
print_success (void)
{
	printf ("  <h1>Welcome</h1>\n"
		"  <p>You have successfully authenticated.</p>\n");
}

static void
print_failure (void)
{
	printf ("  <h1>Try again</h1>\n"
		"  <p>Your username and password could not authenticate you. "
			"You may <a href=\"login.html\">try again</a> or "
			"return to <a href=\"index.html\">the front page</a>."
			"</p>\n");
}

static void
print_footer (void)
{
	printf (" </body>\n"
		"</html>");
}

static char *
strndup (const char *s,
	 size_t len)
{
	char *r;
	char *t;

	t = r = malloc (len + 1);

	while (*s && len--)
	{
		*t++ = *s++;
	}

	*t = '\0';

	return r;
}

/* Extracts a value from the GET line by name */
static char *
extract_val (const char *name)
{
	char *start;
	char *end;
	char *search;

	start = getenv ("QUERY_STRING");
	if (start == NULL) return NULL;

	search = malloc (strlen (name) + 1);
	sprintf (search, "%s=", name);
	start = strstr (start, search);
	free (search);
	if (start == NULL) return NULL;

	start += strlen (name) + 1;

	end = strchr (start, '&');
	if (end == NULL) /* This is the last variable in QUERY_STRING */
	{
		return strdup (start);
	}
	else
	{
		return strndup (start, end - start);
	}
}

int
main (int argc, char **argv)
{
	char *username;
	char *password;

	print_header ();

	username = extract_val ("username");
	if (username == NULL)
	{
		print_failure ();
		print_footer ();
		return 0;
	}
	else
	{
		password = extract_val ("password");
		if (password == NULL)
		{
			free (username);
			print_failure ();
			print_footer ();
			return 0;
		}

		if (authenticate (username, password) == 0)
		{
			print_failure ();
		}
		else
		{
			print_success ();
		}
	}

	print_footer ();
	free (username);
	free (password);

	return 0;
}
