/*
 * Factor: Asks for an integer and prints all the factors.
 *
 * @author Adam Hooper <adamh@densi.com>
 */

import cs1.Keyboard;

public class Factor {
	// {{{ getAFactor()
	/**
	 * Finds and returns a single factor of n.
	 *
	 * @param n A natural number less than or equal to <samp>2</samp>.
	 * @return  The smallest natural factor of n.
	 */
	private static int getAFactor(int n)
	{
		int i;

		for (i = 2; i <= n; i++) {
			if (n % i == 0)
				return i;
		}

		return 1; // Satisfy the compiler
	}

	// }}}
	// {{{ factor()
	/**
	 * Prints all factors of the given natural number.
	 *
	 * @param n Natural number to factor.
	 */
	public static void factor(int n)
	{
		int i;

		while (n > 1) {
			i = getAFactor(n);
			System.out.println(i);
			n /= i;
		}
	}

	// }}}
	// {{{ main()
	/**
	 * Asks for a number and prints its factors.
	 */
	public static void main(String[] args) {
		int input;

		System.out.print("Enter an integer: ");
		input = Keyboard.readInt();

		input = Math.abs(input);

		if (input < 2) {
			System.out.println("Integer must be greater than 2.");
		} else {
			factor(input);
		}
	}

	// }}}
}
