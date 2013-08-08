public class q2 extends basis
{
	public static void main(String[] argv)
	{
		long n;

		if (argv.length != 1)
		{
			System.err.println ("usage: q2 [-m] [-j] [-a]");
			return;
		}

		if (argv[0].equals ("-m"))
		{
			n = find_max_n ("mulmod");
			System.out.println ("Largest value of n with mulmod: " + n);
			return;
		}

		if (argv[0].equals ("-j"))
		{
			n = find_max_n ("mulmod_java");
			System.out.println ("Largest value of n with mulmod_java: " + n);
			return;
		}

		if (argv[0].equals ("-a"))
		{
			n = find_max_n ("mulmod_with_addmod");
			System.out.println ("Largest value of n with mulmod_with_addmod: " +
								n);
			return;
		}

		System.err.println ("usage: q2 [-m] [-j] [-a]");
	}
}
