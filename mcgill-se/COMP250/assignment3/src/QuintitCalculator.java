import java.util.Stack;

public class QuintitCalculator
{
	private Stack digits;
	private Stack operators;

	public QuintitCalculator ()
	{
		digits = new Stack ();
		operators = new Stack ();
	}

	private boolean isQuintit (char c)
	{
		return (c >= '0' && c <= '4');
	}

	private boolean isPushableOperator (char c)
	{
		String s = "+-*/(";

		return s.indexOf (c) != -1;
	}

	private String nextOperator ()
	{
		if (operators.isEmpty ()) return "";

		return (String) operators.peek ();
	}

	private void handlePushableOperator (char c)
	{
		if ((c == '+' || c == '-')
			&& digits.size() > 1
			&& nextOperator ().equals ("(") == false)
		{
			// (a - b + c) == (a - b) + c
			// In this step, we're at the '+' before c 
			// So we'll calculate the (a - b) 
			calculateStep ();
		}

		operators.push ("" + c);
	}

	private void handleChar (char c)
	{
		if (isPushableOperator (c))
		{
			handlePushableOperator (c);
			return;
		}

		if (isQuintit (c))
		{
			digits.push (new Quintit (c - '0'));

			if (operators.isEmpty ()) return;
		}

		if (c == ')')
		{
			if (nextOperator ().equals ("(") == false)
			{
				calculateStep ();
			}

			operators.pop (); // "("
		}

		// We can multiply or divide right away
		if (nextOperator ().equals ("*")
			|| nextOperator ().equals ("/"))
		{
			calculateStep ();
		}
	}

	public Quintit calculate (String s)
	{
		for (int i = 0; i < s.length (); i++)
		{
			handleChar (s.charAt (i));
		}

		if (digits.size () > 1) // Missing a single + or -
		{
			calculateStep ();
		}

		return (Quintit) digits.pop ();
	}

	private void calculateStep ()
	{
		Quintit q1, q2;
		String op;

		q2 = (Quintit) digits.pop ();
		q1 = (Quintit) digits.pop ();

		op = (String) operators.pop ();

		switch (op.charAt (0))
		{
			case '+':
				digits.push (q1.add (q2));
				return;
			case '-':
				digits.push (q1.subtract (q2));
				return;
			case '*':
				digits.push (q1.multiply (q2));
				return;
			case '/':
				digits.push (q1.divide (q2));
				return;
		}

		throw new AssertionError ("Invalid operation");
	}
}
