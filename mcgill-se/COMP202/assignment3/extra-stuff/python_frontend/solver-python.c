#include <Python.h>

#include "solver.h"

static PyObject *
solver_solve(PyObject *args)
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
