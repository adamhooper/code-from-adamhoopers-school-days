#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

gint **triangle;
gint num_rows;

gboolean input_parse(const gchar *filename);
void triangle_alloc(void);
void triangle_free(void);
void triangle_add_row(void);
void triangle_reduce_1_row(void);

// {{{ main()
int main(int argc, char **argv)
{
    gint answer;

    if (argc != 2) {
        g_print("Usage: %s [input file]\n", argv[0]);
        exit(0);
    }

    triangle_alloc();

    if (input_parse(argv[1]) == FALSE) {
        g_print("File '%s' could not be parsed.\n", argv[1]);
        triangle_free();
        exit(1);
    }

    while (num_rows > 1)
        triangle_reduce_1_row();

    answer = triangle[0][0];

    g_print("Largest sum: %d\n", answer);

    triangle_free();

    return 0;
}

// }}}
// {{{ input_parse()
/*
 * Sets up triangle as a 2D array: first dimension is rows, second is columns.
 * The first row will have 1 gint, the second 2, and so on.
 *
 * Returns FALSE if an error occurs.
 */
gboolean input_parse(const gchar *filename)
{
    FILE *fp;
    gchar *line = g_malloc(4096 * sizeof(gchar));
    gint line_num = 0;
    gint i;
    guint64 cur_val;
    gchar** line_split;

    /* Open file */
    if ((fp = fopen(filename, "r")) == NULL) {
        g_print("Could not open file '%s'.\n", filename);
        return FALSE;
    }

    /* Read into array */
    while ((line = fgets(line, 4096, fp))) {
        line_num++;

        line_split = g_strsplit(line, " ", line_num);

        triangle_add_row();

        for (i = 0; i < line_num; i++) {
            if (!line_split[i]) {
                g_print("Line %d of file '%s' too short.\n", line_num,
                         filename);
                return FALSE;
            }

            cur_val = g_ascii_strtoull(line_split[i], NULL, 10);

            if (cur_val == G_MAXUINT64) {
                g_print("Could not parse integer '%s'.\n", line_split[i]);
                return FALSE;
            }

            triangle[line_num-1][i] = cur_val;
        }

        g_strfreev(line_split);
    }

    fclose(fp);

    if (line_num == 0) /* The file was empty or an error occured */
        return FALSE;

    num_rows = line_num;

    return TRUE;
}

// }}}
// {{{ triangle_reduce_1_row()
/**
 * Reduces the bottom row of the triangle, putting the highest value between
 * each pair to the second-bottom row.
 *
 * Undefined behavior if num_rows < 2
 */
void triangle_reduce_1_row(void)
{
    gint row; /* The new bottom row */
    gint i;
    gint l, r;

    row = num_rows - 2;

    for (i = 0; i < row + 1; i++) {
        l = triangle[row + 1][i];
        r = triangle[row + 1][i + 1];

        triangle[row][i] += (l > r ? l : r);
    }

    g_free(triangle[row + 1]);
    num_rows--;
}

// }}}
// {{{ triangle_alloc()
/**
 * Initializes **triangle
 */
void triangle_alloc(void)
{
    triangle = NULL;
    num_rows = 0;
}

// }}}
// {{{ triangle_add_row()
/**
 * Adds (and allocates) a row to the triangle.
 */
void triangle_add_row(void)
{
    num_rows++;

    triangle = realloc(triangle, sizeof(gint *) * num_rows);

    triangle[num_rows - 1] = g_malloc((num_rows) * sizeof (gint));
}

// }}}
// {{{ triangle_free()
/**
 * Frees **triangle
 */
void triangle_free(void)
{
    int i;
    for (i = 0; i < num_rows; i++) {
        g_free(triangle[i]);
    }

    g_free(triangle);
    num_rows = 0;
}

// }}}
