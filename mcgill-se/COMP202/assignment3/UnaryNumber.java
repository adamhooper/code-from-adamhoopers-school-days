//Name: Adam Hooper
//McGIll ID: 260055737
//Course: COMP202
//Section: 002
//Assignment: 3
//Developed at: home
//
//UnaryNumber: A unary number
//
//bugs: divide() by 0 will crash

public class UnaryNumber
{
    private int value;

    // {{{ constructor
    /**
     * Creates a new UnaryNumber from the given string.
	 *
     * @param s must be in the form "IIIIII..."
     */
    public UnaryNumber(String s)
    {
        value = parseUnary(s);
    }

    // }}}
    // {{{ getValue()
    /**
     * @return Unary value of this integer
     */
    public String getValue()
    {
        return createUnary(value);
    }

    // }}}
    // {{{ multiply()
    /**
     * Multiplies this number by the one given.
	 *
     * @param u Number to multiply this one by
     */
    public void multiply(UnaryNumber u)
    {
        value *= parseUnary(u.getValue());
    }

    // }}}
    // {{{ divide()
    /**
     * Divides this number by the given UnaryNumber.
	 *
     * @param u Number to divide this one by
     */
    public void divide(UnaryNumber u)
    {
        value /= parseUnary(u.getValue());
    }

    // }}}
    // {{{ add()
    /**
     * Adds the given UnaryNumber to this one.
	 *
     * @param u Number to add to this one
     */
    public void add(UnaryNumber u)
    {
        value += parseUnary(u.getValue());
    }

    // }}}
    // {{{ subtract()
    /**
     * Subtracts the given number from this one.
	 *
     * @param u Number to subtract from this one
     */
    public void subtract(UnaryNumber u)
    {
        value -= parseUnary(u.getValue());

        if (value < 0)
            value = 0;
    }

    // }}}

    // {{{ parseUnary()
    /**
     * Returns an integer from an "unary" string ("IIII...").
	 *
     * @param  s <code>String</code> representation of <code>UnaryNumber</code>
     * @return   The same number as an <code>integer</code>
     */
    private int parseUnary(String s)
    {
        /*
		 * We don't need this added complexity of error-checking....

        // Parse until the I's end
        int i;
        for (i = 0; i < s.length() && s.charAt(i) == 'I'; i++);
        i--;

        return i;

        */

        return s.length();
    }

    // }}}
    // {{{ createUnary()
    /**
     * From an integer, returns an "unary" string ("III...").
	 *
     * @param  i The <code>integer</code> we want
     * @return A <code>String</code> representation of it
     */
    private String createUnary(int i)
    {
        String ret = "";

        while (i-- > 0)
            ret += "I";

        return ret;
    }

    // }}}
}
