/* vim: set ts=8 sw=8 noet */

public class basis
{
	// Question 1
	// {{{ mulalarusse ()
	/**
	 * Returns the product of the two given numbers.
	 *
	 * The algorithm used is &quot;Multiplication à la Russe&quot;.
	 *
	 * @param a The multiplier
	 * @param b The multiplicand
	 * @return Their product
	 */
	public static long mulalarusse (long a, long b)
	{
		long result = 0;

		while (b != 0)
		{
			if ((b & 1) == 1)
			{
				result += a;
			}

			a <<= 1;
			b >>>= 1;
		}

		return result;
	}

	// }}}

	// Question 2
	// {{{ addmod_java ()
	/**
	 * Returns (a + b % n).
	 *
	 * If a + b exceeds Java's <code>long</code> capacity, addmod_java() will
	 * return -1.
	 */
	public static long addmod_java (long a, long b, long n)
	{
		// Return something impossible if Java can't go that high
		if (Long.MAX_VALUE - a - b < 0) return -1;

		return (a + b) % n;
	}

	// }}}
	// {{{ mulmod_java ()
	/**
	 * Returns (a * b) % n.
	 */
	public static long mulmod_java (long a, long b, long n)
	{
		System.out.println ("a: " + a + ", b: " + b + ", n: " + n
				    + ", (a * b) % n: " + (a * b) % n);
		return (a * b) % n;
	}

	// }}}
	// {{{ mulmod ()
	/**
	 * Returns (a * b) % n via a recursive algorithm.
	 *
	 * The algorithm is similar to that of &quot;Multiplication à la Russe&quot;
	 * but with the mod operation thrown in. To work with a large range of
	 * integers, the mod operation is put after every addition and
	 * multiplication.
	 */
	public static long mulmod (long a, long b, long n)
	{
		long a_doubled;

		if (b == 0) return 0; // a * 0 = 0 %n

		a_doubled = addmod_java (a, a, n);

		if (a_doubled < 0) return -1;

		if ((b & 1) == 0)
		{
			return mulmod (a_doubled, b >>> 1, n);
		}
		else
		{
			return addmod_java (a, mulmod (a_doubled, b >>> 1, n), n);
		}
	}

	// }}}
	// {{{ find_max_n ()
	/**
	 * Finds the maximum integer <code>n</code> for which <code>func</code>
	 * returns a positive number.
	 *
	 * <p>Valid <code>func</code>s:</p>
	 * <ul>
	 *  <li><code>mulmod</code></li>
	 *  <li><code>mulmod_java</code></li>
	 *  <li><code>mulmod_with_addmod</code></li>
	 *  <li><code>addmod</code></li>
	 *  <li><code>addmod_java</code></li>
	 *  <li><code>expmod</code></li>
	 * </ul>
	 *
	 * @param func String name of the method to test.
	 * @return The highest n for which the method returns the correct value,
	 * 		   assuming <code>0 ≤ a, b < n</code>.
	 */
	public static long find_max_n (String func)
	{
		// func accepts paramaters a, b, n
		long a, b, n, jump;
		boolean is_valid;

		jump = (long) 1 << 62;

		for (n = jump; ; n += jump)
		{
			a = b = n - 1;

			// {{{ is_valid = (n > 0 && func (a, b, n) >= 0);
			is_valid = n > 0; // check if n itself exceeds Java's capacity

			if (is_valid)
			{
				if (func.equals ("mulmod"))
				{
					is_valid = mulmod (a, b, n) >= 0;
				}
				else if (func.equals ("mulmod_java"))
				{
					is_valid = mulmod_java (a, b, n) >= 0;
				}
				else if (func.equals ("mulmod_with_addmod"))
				{
					is_valid = mulmod_with_addmod (a, b, n) >= 0;
				}
				else if (func.equals ("addmod"))
				{
					is_valid = addmod (a, b, n) >= 0;
				}
				else if (func.equals ("addmod_java"))
				{
					is_valid = addmod_java (a, b, n) >= 0;
				}
				else if (func.equals ("expmod"))
				{
					is_valid = expmod (a, b, n) >= 0;
				}
			}

			// }}} is_valid

			if (is_valid == false)
			{
				n -= jump; // backtrack to last known good value

				jump >>>= 1;

				if (jump == 0) break; // The previous n was indeed the maximum n

				// So loop -- add (old jump)/2 to n and try that.
			}
		}

		return n;
	}

	// }}}

	// Question 3
	// {{{ addmod ()
	/**
	 * Returns (a + b) % n through a restrictive algorithm.
	 *
	 * As long as <code>0 ≤ a, b < n</code>, this method will return the correct
	 * answer.
	 */
	public static long addmod (long a, long b, long n)
	{
		// assume a < n,  b < n, and all three are positive

		long result = a - n + b;

		if (result >= 0)
		{
			return result;
		}
		else
		{
			return result + n;
		}
	}

	// }}}
	// {{{ mulmod_with_addmod ()
	/**
	 * Same as the <code>mulmod</code> method, but uses <code>addmod</code>
	 * instead of <code>addmod_java</code>.
	 *
	 * @see basis#mulmod(long, long, long)
	 * @see basis#addmod(long, long, long)
	 * @see basis#addmod_java(long, long, long)
	 */
	public static long mulmod_with_addmod (long a, long b, long n)
	{
		long a_doubled;

		if (b == 0) return 0; // a * 0 = 0 %n

		a_doubled = addmod (a, a, n);

		if ((b & 1) == 0)
		{
			// a * b = (2 * a) * (b / 2) % n
			return mulmod_with_addmod (a_doubled,
						   b >>> 1,
						   n);
		}
		else
		{
			// a * b = a + (2 * a) * ((b - 1) / 2) % n
			return addmod (a,
				       mulmod_with_addmod (a_doubled,
					  		   b >>> 1,
							   n),
				       n);
		}
	}

	// }}}

	// Question 4
	// {{{ expmod ()
	/**
	 * Returns (a * b) % n.
	 *
	 * The algorithm used is the same as that in <code>mulmod</code>, but
	 * additions become multiplications.
	 */
	public static long expmod (long a, long b, long n)
	{
		long a_squared;

		if (b == 0) return 1; // a ^ n = 1 % n

		a_squared = mulmod_with_addmod (a, a, n);

		if ((b & 1) == 0)
		{
			// a ^ b = (a ^ 2) ^ (b / 2)
			return expmod (a_squared, b >>> 1, n);
		}
		else
		{
			// a ^ b = a * (a ^ 2) ^ ((b - 1) / 2)
			return mulmod_with_addmod (a,
						   expmod (a_squared,
							   b >>> 1,
							   n),
						   n);
		}
	}

	// }}}
}
