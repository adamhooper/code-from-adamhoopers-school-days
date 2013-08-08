public class Main
{
	public static void main (String[] a)
	{
		Func f;

		if (a[0].equals("fibrec"))
		{
			f = new FibRec ();
		}
		else if (a[0].equals ("fibit"))
		{
			f = new FibIt ();
		}
		else if (a[0].equals ("fibsquare"))
		{
			f = new FibSquare ();
		}
		else if (a[0].equals ("expmod"))
		{
			f = new ExpMod ();
		}
		else if (a[0].equals ("fast"))
		{
			f = new Fast ();
		}
		else
		{
			System.out.println ("Specify an algorithm.");
			return;
		}

		f.printDataPoints ();
	}
}
