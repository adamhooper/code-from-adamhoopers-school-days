public class FibRec extends Func
{
	public FibRec ()
	{
		minN = 0;
		maxN = 35;

		minNumSamples = 200;
		maxNumSamples = 2;
	}

	public int func (long n)
	{
		if (n < 2) return 1;

		return (func (n - 1) % MOD_CONST
			+ func (n - 2) % MOD_CONST)
		       % MOD_CONST;
	}
}
