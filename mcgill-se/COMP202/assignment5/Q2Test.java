

/**
 * Q2Test: program for testing correct working of the ArraySorter class.  If
 *         the ArraySorter class works correctly, then this program will
 *         print out its array in properly sorted order.
 **/

public class Q2Test {

    public static void main(String[] argv) {

	int[][] testArray = {  {1, 6, 3, 11}, 
			       {4, 7, 9, 5}, 
			       {5, 2, 8, 16}, 
	                    };


	System.out.println("Initial Array: ");
	printArray(testArray);

	ArraySorter.sort2DArray(testArray);

	System.out.println("\nSorted Array: ");
	printArray(testArray);
    }



    /**
     * Helper method: prints out a two-dimensional integer array in a "nice"
     * way.  This works fine as long as numbers are between 1 and 99
     **/
    public static void printArray(int [][] a) {
	if (a == null)
	    return;
	else {
	    for (int y=0; y < a.length; y++) {
		for (int x=0; x < a[y].length; x++) {
		    System.out.print(a[y][x] + " ");
		    if (a[y][x] < 10) 
			System.out.print(" ");
		}
		System.out.println("");
	    }

	}  // end else

    } // end printArray method

}  // end Q2Test class
