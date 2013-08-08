public class Main
{
	public static void main (String[] a)
	{
		QuintitCalculator c;
		Quintit q;

		if (a.length != 1)
		{
			System.err.println ("usage: Main [expression]");
			return;
		}

		c = new QuintitCalculator ();
		try {
			q = c.calculate (a[0]);
		}
		catch (ArithmeticException e)
		{
			System.err.println ("DIVISION BY ZERO");
			return;
		}

		System.out.println (q);
	}
}
