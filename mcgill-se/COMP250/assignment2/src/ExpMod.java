public class ExpMod extends Func
{
	public ExpMod ()
	{
		minN = 0;
		maxN = Long.MAX_VALUE;
		iterN = Long.MAX_VALUE >> 7;

		minNumSamples = 100;
		maxNumSamples = 100;
	}

	public int func (long n)
	{
		if (n < 2) return 1;

		Matrix2x2 m = new Matrix2x2 (0, 1, 1);

		m = Matrix2x2.ExpMod (m, n);

		return m.d;
	}
}

class Matrix2x2
{
	/*
	 * [ a  b ]
	 * [ c  d ]
	 *
	 * c is *always* equal to b. Therefore, to optimize all the functions
	 * ahead, "b" is used any place "c" would be needed.
	 */
	public int a, b, d;

	private static int addmod (int a, int b, int n)
	{
		// assume a < n,  b < n, and all three are positive

		int result = a - n + b;

		if (result >= 0)
		{
			return result;
		}
		else
		{
			return result + n;
		}
	}

	private static int mulmod (int a, long b, int n)
	{
		int a_doubled;

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

	public Matrix2x2 (int a, int b, int d)
	{
		this.a = a;
		this.b = b;
		this.d = d;
	}

	public Matrix2x2 square ()
	{
		return Multiply (this);
	}

	public Matrix2x2 Multiply (Matrix2x2 m)
	{
		Matrix2x2 ret = new Matrix2x2 (
				addmod (mulmod (a, m.a, Func.MOD_CONST),
					mulmod (b, m.b, Func.MOD_CONST),
					Func.MOD_CONST),
				addmod (mulmod (a, m.b, Func.MOD_CONST),
					mulmod (b, m.d, Func.MOD_CONST),
					Func.MOD_CONST),
				addmod (mulmod (b, m.b, Func.MOD_CONST),
					mulmod (d, m.d, Func.MOD_CONST),
					Func.MOD_CONST)
				);
		return ret;
	}

	public static Matrix2x2 ExpMod (Matrix2x2 m, long b)
	{
		if (b == 0) return new Matrix2x2 (1, 0, 1);

		Matrix2x2 half_squared = ExpMod (m.square (), b >> 1);

		if ((b & 1) == 0)
		{
			return half_squared;
		}
		else
		{
			return half_squared.Multiply (m);
		}
	}
}
