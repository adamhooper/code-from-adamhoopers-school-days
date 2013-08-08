//Name: Adam Hooper
//McGIll ID: 260055737
//Course: COMP202
//Section: 002
//Assignment: 3
//Developed at: home
//
//ArithmeticTable: Prints a table in which each cell is col [operation] row

public class ArithmeticTable
{
    private int tableMin, tableMax;
    private char tableOperation;

    // {{{ constructor
    /**
	 * Initializes an ArithmeticTable.
	 *
	 * @param min       Minimum row/column value in the table
	 *        max       Maximum row/column value in the table
	 *        operation Either '+' or '*', defaults to '*'
     */
    public ArithmeticTable(int min, int max, char operation)
    {
        // Make sure min is less than max
        if (min > max) { int x = min; min = max; max = x; }

        // operation should be '+' or '*'
        if (operation != '+') operation = '*';

        tableMin = min;
        tableMax = max;
        tableOperation = operation;
    }

    // }}}
    // {{{ getMin()
    /**
     * @return Minimum value of row/columns in table
     */
    public int getMin()
    {
        return tableMin;
    }

    // }}}
    // {{{ getMax()
    /**
     * @return Maximum value of row/columns in table
     */
    public int getMax()
    {
        return tableMax;
    }

    // }}}
    // {{{ printTable()
    /**
     * Prints an arithmetic table.
     */
    public void printTable()
    {
        int row;
        int col;
        int cell;


        // Print column headers
        System.out.print("   ");
        for (col = tableMin; col < tableMax; col++) {
            System.out.print(spaceNum(col) + " ");
        }
        System.out.println(col + "\n");

        // Loop through rows
        for (row = tableMin; row <= tableMax; row++) {
            // Title
            System.out.print(spaceNum(row));

            // Data
            for (col = tableMin; col < tableMax; col++ ){
                cell = operate(row, col);

                System.out.print(spaceNum(cell) + " ");
            }

            cell = operate(row, col);
            System.out.println(cell);
        } // for (row = tableMin; row <= tableMax; row++)
    }

    // }}}
    // {{{ spaceNum()
    /**
     * Returns the number as a string padded with spaces
     * @param int n The number to format
     * @return String A String of the number, padded with spaces
     */
    private String spaceNum(int n)
    {
        if (n > 99)
            return n + "";
        if (n > 9)
            return n + " ";
        if (n > 0)
            return n + "  ";
        return "ERR";
    }

    // }}}
    // {{{ operate()
    /**
     * Runs the desired operation on the two given integers.
	 *
	 * @param n1 First number
	 *        n2 Second number
     * @return int n1 [tableOperation] n2
     */
    private int operate(int n1, int n2)
    {
        if (tableOperation == '+')
            return n1 + n2;

        return n1 * n2;
    }

    // }}}
}
