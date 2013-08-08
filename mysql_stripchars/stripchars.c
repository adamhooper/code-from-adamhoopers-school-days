#include <stdio.h>
#include <string.h>
#include <my_global.h>
#include <my_sys.h>
#include <mysql.h>

/* Default arguments to stripchars() */
const char *defaults[] = {"", "-./,:()\\*", ""};

my_bool stripchars_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
  
  /* Check number of arguments. */
  if(args->arg_count == 0 || args->arg_count > 3) {
    strcpy(message, "Wrong number of arguments.");
    return 1;
  }

  /* Make sure it returns a string */
  if(args->arg_type[0] != STRING_RESULT) {
    strcpy(message, "arg_type[0] != STRING_RESULT");
    return 1;
  }

  /* Copy defaults to initid->ptr, the shared data pointer */
  initid->ptr = malloc(sizeof(defaults));
  if(!initid->ptr) {
      strcpy(message, "malloc() failed.");
      return 1;
  }
  memcpy(initid->ptr, defaults, sizeof(defaults));

  return 0;
}

void stripchars_deinit(UDF_INIT *initid) {
  free(initid->ptr);
}

/* The actual function, not what's called from MySQL. */
void _stripchars(char *result, const char *string, char *to_strip, char *repl_with) {
  char lookup[256];

  if (strlen(repl_with) > 0) { /* "Translate" function */
    int i;

    /* lookup is a translation table */
    for(i = 0; i < 256; i++)
      lookup[i] = i;

    /* Take parameters 3 and 4 and put them into the translation table */
    while(*to_strip && *repl_with)
      lookup[(int)*to_strip++] = *repl_with++;

    /* Loop through parameter 2, replacing every value with the translated one */
    while((*result++ = lookup[(int)*string++]));

  } else { /* "Strip" function */

    /* lookup is an array of true/false ("should this char not be copied?") */
    memset(lookup, 0, 256);
  
    /* Set which characters shouldn't be copied */
    while(*to_strip)
      lookup[(int)*to_strip++] = 1;

    /* Loop through param 2, inserting into param 1 anything not marked for exclusion */
    do {
      if(!lookup[(int)*string])
        *result++ = *string;
    } while (*string++);
  }
}

/* The MySQL wrapper to _stripchars() */
char *stripchars(UDF_INIT *initid,
    UDF_ARGS *args,
    char *result,
    unsigned long *length,
    char *is_null,
    char *error)
{
  int i;
  /* Fill in all arguments with the defaults in initid->ptr */
  char **tmp = ((char **)initid->ptr);

  /* Replace the defaults with arguments that WERE passed to it (in SQL) */
  for(i = 0; i < args->arg_count; i++) {
    tmp[i] = args->args[i];
  }

  /* Run the actual function, modifying char *result */
  _stripchars(result, tmp[0], tmp[1], tmp[2]);

  *length = strlen(result);
  return result;
}

/* Test function if not compiled as a shared library */
int main() {
  char *s = "Escape me";
  char *ret = malloc(sizeof(s));

  _stripchars(ret, s, "e", "b");

  printf("\"%s\"\n", ret);

  return 0;
}
