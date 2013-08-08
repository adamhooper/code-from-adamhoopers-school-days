public class FibSquare extends Func
{
	public FibSquare ()
	{
		minN = 0;
		maxN = 4000000;
		iterN = 100000;

		minNumSamples = 4;
		maxNumSamples = 1;
	}

	private int square (long a)
	{
		return mulmod (a, a, MOD_CONST);
	}

	private int addmod (long a, long b, long n)
	{
		// assume a < n,  b < n, and all three are positive

		long result = a - n + b;

		if (result >= 0)
		{
			return (int) result;
		}
		else
		{
			return (int) (result + n);
		}
	}

	private int mulmod (long a, long b, int n)
	{
		long a_doubled;

		if (b == 0) return 0; // a * 0 = 0 %n

		a_doubled = addmod (a, a, n);

		if ((b & 1) == 0)
		{
			// a * b = (2 * a) * (b / 2) % n
			return mulmod (a_doubled, b >>> 1, n);
		}
		else
		{
			// a * b = a + (2 * a) * ((b - 1) / 2) % n
			return addmod (a, mulmod (a_doubled, b >>> 1, n), n);
		}
	}

	public int func (long n)
	{
		if (n < 3) return 1;

		if ((n & 1) == 1)
		{
			return addmod (square (func ((n + 1) / 2)),
				       square (func ((n - 1) / 2)),
				       MOD_CONST);
		}
		else
		{
			return addmod (square (func (n / 2 + 1)),
				       -square (func (n / 2 - 1)),
				       MOD_CONST);
		}
	}
}
