public class FibIt extends Func
{
	public FibIt ()
	{
		minN = 0;
		iterN = 1000000;
		maxN = 45000000;

		minNumSamples = 1;
		maxNumSamples = 1;
	}

	public int func (long n)
	{
		int f = 1, g = 1;
		int t;

		while (--n > 0)
		{
			t = f;
			f = (f + g) % MOD_CONST;
			g = t;
		}

		return f;
	}
}
