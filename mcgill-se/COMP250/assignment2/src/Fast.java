import java.util.Vector;

public class Fast extends ExpMod
{
	private int period = 0;

	public Fast ()
	{
		MOD_CONST = 1000;

		minN = 0;
		maxN = 4000;
		iterN = 2;

		maxNumSamples = 1000;
		minNumSamples = 1000;

		period = find_period ();
	}

	public int func (long n)
	{
		return super.func (n % period);
	}

	private int find_period ()
	{
		// Iterate as with FibIt, and stop when we hit 0 and 1 again
		int f = 1, g = 1;
		int t;
		int ret = 0;

		while (true)
		{
			ret++;

			if (g % MOD_CONST == 0 && f % MOD_CONST == 1)
			{
				return ret;
			}

			t = f;
			f = (f + g) % MOD_CONST;
			g = t;
		}
	}
}
