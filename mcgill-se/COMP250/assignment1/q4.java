public class q4 extends basis
{
	public static void main (String[] argv)
	{
		long a, b, c, d, e, n;

		a = 1274434408442L;
		b = 589394265617L;
		n = 1606818609763L;

		System.out.println ("a: " + a);
		System.out.println ("b: " + b);
		System.out.println ("n: " + n);

		System.out.println ("c = (a ^ b) % n");
		c = expmod (a, b, n);
		System.out.println ("c: " + c);

		d = 433371342353L;

		System.out.println ("d: " + d);

		System.out.println ("e = (c ^ d) % n");
		e = expmod (c, d, n);
		System.out.println ("e: " + e);
	}
}
