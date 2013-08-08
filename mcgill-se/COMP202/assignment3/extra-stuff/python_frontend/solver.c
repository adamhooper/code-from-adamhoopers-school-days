#include <Python.h>

#include "solver.h"

static PyObject *
solver_solve(PyObject *self, PyObject *args)
{
  char *question;
  char *solution;

  if (!PyArg_ParseTuple(args, "s", &question))
    return NULL;

  solution = solve_equation(question);

  return Py_BuildValue("s", solution);
}

static PyMethodDef SolverMethods[] = {
  {"solve", solver_solve, METH_VARARGS,
   "Find an equation for the given string of digits." },
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initsolver(void)
{
  (void) Py_InitModule("solver", SolverMethods);
}

#include <glib.h>
#include <glib/gprintf.h>

void print_usage(void);
gchar *match_lhs_rhs(gchar *lhs, gchar *rhs);
GSList *get_left_results(gchar *lhs);
GSList *list_combinations(gchar *digits);
GSList *permute(gchar first_char, GSList *list);
gboolean is_first_number_zero(gchar *s);
gint64 get_value(gchar *s);
gchar *find_rhs(gchar *rhs, gchar *rhs_pos, gint64 val);

/* {{{ solve_equation() */
gchar *solve_equation(gchar *digits)
{
  gchar *solution;
  gchar *lhs, *rhs;
  gchar *c;

  for (c = digits; *++c;) {
    lhs = g_strndup(digits, c - digits);
    rhs = g_strdup(c);

    solution = match_lhs_rhs(lhs, rhs);

    g_free(lhs);
    g_free(rhs);

    if (solution)
      return solution;
  }

  return NULL;
}

/* }}} */
/* {{{ match_lhs_rhs() */
gchar *match_lhs_rhs(gchar *lhs, gchar *rhs)
{
  GSList *left_results;
  GSList *lefti;
  combo_val *combo;
  gchar *right_str;
  GString *ret;
  gchar *ret_chars;

  left_results = get_left_results(lhs);

  for (lefti = left_results; lefti; lefti = g_slist_next(lefti)) {
    combo = (combo_val*)lefti->data;

    right_str = find_rhs(rhs, rhs, combo->val);

    if (right_str) {
      ret = g_string_new(combo->str);
      ret = g_string_append_c(ret, '=');
      ret = g_string_append(ret, right_str);

      g_free(combo->str);
      g_free(combo);

      g_free(right_str);

      ret_chars = ret->str;
      g_string_free(ret, FALSE);

      return ret_chars;
    }

    g_free(combo->str);
    g_free(combo);
  }

  for (; lefti; lefti = g_slist_next(lefti)) {
    g_free(((combo_val*)lefti->data)->str);
    g_free(lefti->data);
  }

  g_slist_free(left_results);

  return NULL;
}

/* }}} */
/* {{{ find_rhs() */
gchar *find_rhs(gchar *rhs, gchar *rhs_pos, gint64 val)
{
  gchar *ret;
  GString *s;
  gchar *s_char;

  if (get_value(rhs) == val)
    return g_strdup(rhs);

  if (!*(rhs_pos+1))
    return NULL;

  ret = find_rhs(rhs, rhs_pos + 1, val);
  if (ret)
    return ret;

  if (*(rhs_pos + 1) != '0') {
    s = g_string_new_len(rhs, rhs_pos - rhs + 1);
    s = g_string_append_c(s, '/');
    s = g_string_append(s, rhs_pos + 1);

    s_char = s->str;
    g_string_free(s, FALSE);

    ret = find_rhs(s_char, s_char + (rhs_pos + 2 - rhs), val);
    g_free(s_char);

    if (ret)
      return ret;
  }

  return NULL;
}

/* }}} */
/* {{{ get_left_results() */
GSList *get_left_results(gchar *lhs)
{
  GSList *combinations;
  GSList *l;
  gint val;
  GSList *ret = NULL;
  GSList *reti;
  combo_val *combo;
  gboolean skip;

  combinations = list_combinations(lhs);

  for (l = combinations; l; l = g_slist_next(l)) {
    skip = FALSE;
    val = get_value(l->data);

    /* Make sure there's no duplication */
    for (reti = ret; reti; reti = g_slist_next(reti)) {
      if (val == ((combo_val*)(reti->data))->val) {
        skip = TRUE;
        break;
      }
    }

    if (!skip) {
      combo = g_malloc(sizeof(combo_val));
      combo->val = val;
      combo->str = l->data;

      ret = g_slist_prepend(ret, combo);
    } else {
      g_free(l->data);
    }
  }

  /* Check memory? */
  g_slist_free(combinations);

  return ret;
}

/* }}} */
/* {{{ list_combinations() */
GSList *list_combinations(gchar *digits) {
  GSList *ret = NULL;

  if (!*(digits+1)) {
    /* Return a single-value GSList */
    ret = g_slist_append(ret, g_strdup(digits));
    return ret;
  }

  ret = list_combinations(digits + 1);

  ret = permute(*digits, ret);

  return ret;
}

/* }}} */
/* {{{ permute() */
GSList *permute(gchar first_char, GSList *list) {
  GSList *new_list = NULL;
  GSList *t_list = list;
  GString *s = NULL;
  gchar *travel_string;

  s = g_string_new("");

  for (t_list = list; t_list; t_list = g_slist_next(t_list)) {

    travel_string = t_list->data;

    if (!is_first_number_zero(travel_string)) {
      g_string_printf(s, "%c/%s", first_char, travel_string);
      new_list = g_slist_prepend(new_list, g_strdup(s->str));

      g_string_printf(s, "%c%%%s", first_char, travel_string);
      new_list = g_slist_prepend(new_list, g_strdup(s->str));
    }

    g_string_printf(s, "%c%s", first_char, travel_string);
    new_list = g_slist_prepend(new_list, g_strdup(s->str));

    g_string_printf(s, "%c+%s", first_char, travel_string);
    new_list = g_slist_prepend(new_list, g_strdup(s->str));

    g_string_printf(s, "%c-%s", first_char, travel_string);
    new_list = g_slist_prepend(new_list, g_strdup(s->str));

    g_string_printf(s, "%c*%s", first_char, travel_string);
    new_list = g_slist_prepend(new_list, g_strdup(s->str));

    g_free(t_list->data);
  }

  g_string_free(s, TRUE);

  g_slist_free(list);

  return new_list;
}

/* }}} */
/* {{{ is_first_number_zero() */
gboolean is_first_number_zero(gchar *s)
{
  for (; *s; s++) {
    if (*s != '0') {
      if (g_ascii_isdigit(*s))
        return FALSE;
      break;
    }
  }
  return TRUE;
}

/* }}} */
/* {{{ get_value() */
gint64 get_value(gchar *s) {
  gint64 l, r;
  char op;

  l = g_ascii_strtoull(s, NULL, 10);

  while (1) {
    /* Skip the number we just read */
    for (; g_ascii_isdigit(*s); s++);

    if (*s == '\0')
      return l;

    /* Store operator and number */
    op = *s++;
    r = g_ascii_strtoull(s, NULL, 10);

    switch (op) {
      case '+':
        l += r;
        break;
      case '-':
        l -= r;
        break;
      case '*':
        l *= r;
        break;
      case '/':
        l /= r;
        break;
      case '%':
        l %= r;
        break;
    }
  }
}

/* }}} */
