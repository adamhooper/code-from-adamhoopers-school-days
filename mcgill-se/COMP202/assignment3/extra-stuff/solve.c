#include <stdlib.h>

#include <glib.h>
#include <glib/gprintf.h>

#include "solve.h"

void print_usage(void);

/* {{{ main() */
int main(int argc, char **argv) {
  gchar *c;
  gchar *input;
  gchar *solution;

  /* Right number of arguments? */
  if (argc != 2) {
    print_usage();
    exit(0);
  }

  input = argv[1];

  /* Validate input */
  for (c = input; *c; c++) {
    if (!(*c >= '0' && *c <= '9')) {
      print_usage();
      exit(0);
    }
  }

  if (c - input < 2) {
    print_usage();
    exit(0);
  }

  /* solve */
  solution = solve_equation(input);

  if (solution)
    g_printf("Solution: %s\n", solution);
  else
    g_printf("No solution was found for string %s.\n", input);

  return 0;
}

/* }}} */
/* {{{ print_usage() */
void print_usage(void)
{
  g_printf("Usage: ./solve [string-of-digits]\n");
}

/* }}} */
