// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 5
// Developed at: Home
//
// EqualSplitter: Prints the given array into two equal-summed lists.

import java.util.Arrays;

public class EqualSplitter
{
    // {{{ equalSplit()
    /**
     * Prints the given list of integers as two lists whose sums are identical.
     *
     * For example, {11, 10, 8, 7, 2, 4} will print [11+8+2] and [10+7+4].
     *
     * @param list List of integers.
     */
    public void equalSplit(int[] list)
    {
        int sum; // Sum of all integers in the list.
        int target; // The number half the list should add up to.
        int[][] possibleSubLists = getPossibleSubLists(list);

        sum = sum(list);
        // If sum is odd, we can't solve.
        if (sum % 2 != 0) {
            printFailure();
            return;
        }

        target = sum / 2;

        removeListsDuplicates(list, possibleSubLists);

        // Remove all possible sublists that don't add up to target.
        removeListsWrongSum(possibleSubLists, target);

        // Print what's left.
        printSolutions(list, possibleSubLists);
    }

    // }}}

    // {{{ getOtherSubList()
    /**
     * Given a full list and a sub-list of it, returns the part of list not
     * contained within the sub-list.
     *
     * @param list Full list of integers.
     * @param sub  Sub-list of the full list.
     * @return     The elements of list not found in sub.
     */
    private int[] getOtherSubList(int[] list, int[] sub)
    {
        int[] subCopy = new int[sub.length];
        int[] ret = new int[list.length];
        int dupPos;

        for (int i = 0; i < sub.length; i++)
            subCopy[i] = sub[i];

        for (int i = 0; i < list.length; i++) {
            dupPos = search(subCopy, list[i]);
            if (dupPos >= 0) {
                subCopy[dupPos] = 0;
                ret[i] = 0;
            } else {
                ret[i] = list[i];
            }
        }

        moveZeros(ret);

        return ret;
    }

    // }}}
    // {{{ getPossibleSubLists()
    /**
     * Returns a 2-d array, each element being a possible sub-list of the input.
     *
     * Sub-lists will be padded with 0's at the ends.
     *
     * @param a Array of integers.
     * @return  2D array, each element being an array of some integers from a.
     */
    private int[][] getPossibleSubLists(int[] a)
    {
        /**
         * For example, the list [1,2,3] will be exploded as follows:
         *
         *      3
         *     /
         *    2
         *   / \
         *  /   0
         * 1 
         *  \   3
         *   \ /
         *    0
         *     \
         *      0
         *
         *      3
         *     /
         *    2
         *   / \
         *  /   0
         * 0 
         *  \   3
         *   \ /
         *    0
         *     \
         *      0
         *
         * Reading these out, we get:
         * [1,2,3]
         * [1,2,0]
         * [1,0,3]
         * [1,0,0]
         * [0,2,3]
         * [0,2,0]
         * [0,0,3]
         * [0,0,0]
         *
         * This method fills a column at a time, not a row at a time.
         */

        int[][] ret = new int[(int) Math.pow(2, a.length)][a.length];

        for (int col = 0; col < a.length; col++) {
            boolean on = false;
            int numUntilSwitchOn = 0;

            for (int row = 0; row < (int) Math.pow(2, a.length); row++) {
                if (--numUntilSwitchOn < 1) {
                    numUntilSwitchOn = (int) Math.pow(2, a.length - 1 - col);
                    on = !on;
                }

                ret[row][col] = on ? a[col] : 0;
            }
        }

        // Move the 0's to the end
        for (int row = 0; row < ret.length; row++) {
            moveZeros(ret[row]);
        }

        return ret;
    }

    // }}}
    // {{{ arrayAsString()
    /**
     * Returns the given list as a string sum (i.e., "1+2+3").
     *
     * @param a List of integers to print.
     * @return  The list as a sum.
     */
    private String arrayAsString(int[] a)
    {
        String ret = "[";
        boolean first = true;

        for (int i = 0; i < a.length; i++) {
            if (a[i] == 0)
                break;

            if (!first)
                ret += ",";
            else
                first = false;

            ret += a[i];
        }

        ret += "]";

        return ret;
    }

    // }}}
    // {{{ moveZeros()
    /**
     * Reorders the given array so its 0's are at the end.
     *
     * For example, [1,0,3] will become [1,3,0].
     *
     * @param a Array to re-order
     */
    private void moveZeros(int[] a)
    {
        for (int i = 0; i < a.length; i++) {
            if (a[i] != 0)
                continue;

            for (int j = i; j < a.length; j++) {
                if (a[j] != 0) {
                    a[i] = a[j];
                    a[j] = 0;
                    break;
                }
            }

            // If the rest of the array is zeros, return
            if (a[i] == 0)
                return;
        }

        return;
    }

    // }}}
    // {{{ sum()
    /**
     * Returns the sum of all values in the given array.
     *
     * @param a List of integers.
     * @return  Their sum.
     */
    private int sum(int[] a)
    {
        int ret = 0;

        for (int i = 0; i < a.length; i++)
            ret += a[i];

        return ret;
    }

    // }}}
    // {{{ removeListsDuplicates()
    /**
     * Removes duplicate rows from the given 2D array.
     *
     * @param a 2D array of integers.
     */
    private void removeListsDuplicates(int[] list, int[][] sols)
    {
        int[] otherSubList;

        for (int i = 0; i < sols.length; i++) {
            if (sols[i] == null)
                continue;

            otherSubList = getOtherSubList(list, sols[i]);

            for (int j = i + 1; j < sols.length; j++) {
                if (Arrays.equals(sols[i], sols[j]) ||
                        Arrays.equals(otherSubList, sols[j]))
                    sols[j] = null;
            }
        }
    }

    // }}}
    // {{{ removeListsWrongSum()
    /**
     * Removes rows from the given 2D array which do not have the target sum.
     *
     * @param a 2D array of integers.
     * @param s Target sum.
     */
    private void removeListsWrongSum(int[][] a, int s)
    {
        for (int i = 0; i < a.length; i++) {
            if (a[i] == null)
                continue;

            if (sum(a[i]) != s)
                a[i] = null;
        }
    }

    // }}}
    // {{{ printFailure()
    /**
     * Prints that no solution could be found.
     */
    private void printFailure()
    {
        System.out.println("Could not create an equation.");
    }

    // }}}
    // {{{ printSolutions()
    /**
     * Prints the given solutions found for the given list.
     *
     * @param list The question, an array of integers.
     * @param sols The solutions, array of sub-lists of list.
     */
    private void printSolutions(int[] list, int[][] sols)
    {
        int[] otherSubList;
        int numSols = 0;

        for (int i = 0; i < sols.length; i++) {
            if (sols[i] == null)
                continue;

            numSols++;

            otherSubList = getOtherSubList(list, sols[i]);

            System.out.println("Solution " + numSols + ": " +
                               arrayAsString(sols[i]) + " and " +
                               arrayAsString(otherSubList));
        }

        if (numSols == 0) {
            printFailure();
            return;
        }

        System.out.println(numSols + " solution" + ((numSols == 1) ? "" : "s") +
                           " found.");
    }

    // }}}
    // {{{ search()
    /**
     * Searches the given array for the given integer, and returns its key.
     *
     * @param a Array to search.
     * @param i Integer to find.
     * @return  i's position in a, or -1 if i is not anywhere in a.
     */
    private int search(int[] a, int i)
    {
        for (int j = 0; j < a.length; j++) {
            if (a[j] == i)
                return j;
        }

        return -1;
    }

    // }}}
}
