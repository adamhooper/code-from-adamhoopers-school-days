public class q1 extends basis
{
	public static void main(String[] argv)
	{
		long a, b;

		if (argv.length != 2)
		{
			System.err.println ("usage: q1 [multiplier] [multiplicand]");
			return;
		}

		a = Long.parseLong (argv[0]);
		b = Long.parseLong (argv[1]);

		System.out.println (a + " * " + b + " = " + mulalarusse (a, b));
	}
}
