#include <stdlib.h>

#include <glib.h>
#include <glib/gprintf.h>

#include "solve.h"

void print_usage(void);

int main(int argc, char **argv)
{
  guint64 start, end;
  guint64 i;
  GString *question;
  gchar *solution;

  if (argc != 3) {
    print_usage();
  }

  start = g_ascii_strtoull(argv[1], NULL, 10);
  end   = g_ascii_strtoull(argv[2], NULL, 10);

  if (end < start) {
    print_usage();
  }

  question = g_string_new("");

  for (i = start; i <= end; i++) {
    g_string_printf(question, "%lld", i);

    solution = solve_equation(question->str);

    g_printf("%s: %s\n", question->str, solution ? solution : "No solution");

    if (solution)
      g_free(solution);
  }

  g_string_free(question, TRUE);

  return 0;
}

void print_usage(void)
{
  g_printf("Usage: ./torture [start_num] [end_num]\n");
  exit(0);
}
