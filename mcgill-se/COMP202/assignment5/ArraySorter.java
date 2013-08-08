// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 5
// Developed at: Home
//
// ArraySorter: Sorts a 2D array.

public class ArraySorter
{
    // {{{ sort2DArray()
    /**
     * Sorts a 2D integer array.
     *
     * <p>An array is sorted if all its rows and all its columns are sorted.</p>
     *
     * <p>This method uses an interesting sorting property: sorting can be
     *    completed in one pass. Sort all rows, then sort all columns,
     *    and you're done. I won't prove it, but just think about it: the
     *    top-left cell will invariably have the smallest number, etc.</p>
     *
     * @param c Unsorted array.
     */
    public static void sort2DArray(int[][] c)
    {
        int row;
        int col;
        int i;
        int minPos;
        int t;

        if (c == null) return;

        // Sort rows
        for (row = 0; row < c.length; row++) {
            // selection sort
            //
            // It'd be nice to put this in its own method, but the question
            // doesn't allow it.
            for (col = 0; col < c[row].length; col++) {
                minPos = col;

                for (i = col; i < c[row].length; i++) {
                    if (c[row][i] < c[row][minPos])
                        minPos = i;
                }

                // swap
                t = c[row][col];
                c[row][col] = c[row][minPos];
                c[row][minPos] = t;
            }
        }

        // Sort cols
        for (col = 0; col < c[0].length; col++) {
            // selection sort, again.
            for (row = 0; row < c.length; row++) {
                minPos = row;

                for (i = row; i < c.length; i++) {
                    if (c[i][col] < c[minPos][col])
                        minPos = i;
                }

                // swap
                t = c[row][col];
                c[row][col] = c[minPos][col];
                c[minPos][col] = t;
            }
        }
    }

    // }}}
}
