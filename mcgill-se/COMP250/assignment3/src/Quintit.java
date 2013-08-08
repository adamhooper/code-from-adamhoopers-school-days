public class Quintit
{
	private int val;

	public Quintit (int i)
	{
		val = i;
	}

	public Quintit (String s)
	{
		val = Integer.parseInt (s);
	}

	public Quintit add (Quintit q)
	{
		return new Quintit ((val + q.val) % 5);
	}

	public Quintit subtract (Quintit q)
	{
		return new Quintit ((5 + val - q.val) % 5);
	}

	public Quintit multiply (Quintit q)
	{
		return new Quintit (val * q.val % 5);
	}

	public Quintit divide (Quintit q)
	{
		for (byte _b = 0; _b < 5; _b++)
		{
			Quintit _q = new Quintit (_b);
			if (q.multiply (_q).val == 1)
			{
				return multiply (_q);
			}
		}

		throw new ArithmeticException ("Divide by zero");
	}

	public String toString ()
	{
		return "" + val;
	}
}
