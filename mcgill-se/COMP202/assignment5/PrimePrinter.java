// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 5
// Developed at: Home
//
// PrimePrinter: Finds primes up to the given integer.

import cs1.Keyboard;

public class PrimePrinter
{
    // {{{ printPrimes()
    /**
     * Prints all prime numbers up to the given integer.
     *
     * The method is simple: Take an array of booleans the size of the integer
     * plus one, all true. Assume 2 is prime, cross off any multiples. Assume 3
     * is prime, cross off any multiples. Assume 4 is prime (don't worry, it's
     * been crossed off already), and cross off any multiples. Etc. "Crossing
     * off" means setting the array element to false. When finished, the array
     * has true values only for prime numbers: print them.
     *
     * @param max Maximum number (all primes printed will be &lt;= max).
     */
    public static void printPrimes(int max)
    {
        int i, j;
        boolean first = true; // A hack to avoid printing an extra " "

        boolean[] primes = new boolean[max + 1];

        // Set all to true
        for (i = 0; i <= max; i++)
            primes[i] = true;

        // Exclude all non-primes
        primes[0] = false;
        primes[1] = false;

        for (i = 2; i <= max; i++)
            for (j = i*2; j <= max; j += i)
                primes[j] = false;

        // Print the stuff that is still true.
        for (i = 0; i <= max; i++) {
            if (!primes[i])
                continue;

            if (!first)
                System.out.print(" ");
            else
                first = false;

            System.out.print(i);
        }

        System.out.println();
    }

    // }}}
    // {{{ main()
    /**
     * Asks the user for an integer, prints all primes up to that integer.
     */
    public static void main(String[] argv)
    {
        System.out.print("Enter a number: ");
        int k = Keyboard.readInt();

        if (k < 1) 
            k = 2;

        System.out.println("Prime numbers from 1 to " + k + ":");
        printPrimes(k);
    }

    // }}}
}
