public abstract class Func
{
	public static int MOD_CONST = 3333373;

	protected long minN = 0;
	protected long iterN = 1;
	protected long maxN = 0;

	protected int maxNumSamples = 0;
	protected int minNumSamples = 0;

	public abstract int func (long n);

	public double time (long n, int num_samples)
	{
		long startTime, endTime;
		int i = num_samples;

		startTime = System.currentTimeMillis ();

		while (i-- > 0)
		{
			func (n);
		}

		endTime = System.currentTimeMillis ();

		return (double) (endTime - startTime) / num_samples;
	}

	public double time (long n) { return time (n, 1); }

	private int findNumSamples (long n)
	{
		if (maxN == minN) return minNumSamples;

		double slope = (double) (maxNumSamples - minNumSamples)
			       / (maxN - minN);

		return (int) ((n - minN) * slope + minNumSamples);
	}

	public void printDataPoints ()
	{
		int numSamples;

		for (long n = minN; n <= maxN && n >= 0; n += iterN)
		{
			numSamples = findNumSamples (n);

			System.out.println (n + " " + time (n, numSamples));
		}
	}
}
