//Name: Adam Hooper
//McGIll ID: 260055737
//Course: COMP202
//Section: 002
//Assignment: 3
//Developed at: home
//
//UnaryMath: Mathematical operations with UnaryNumber's

public class UnaryMath
{
    // {{{ abs()
    /**
     * Returns the absolute value of u1 - u2.
	 *
     * @param  u1 First number
     * @param  u2 Second number
     * @return    abs(u1-u2)
     */
    public static UnaryNumber abs(UnaryNumber u1, UnaryNumber u2)
    {
        int i1, i2;
        int val;

        i1 = convertToDecimal(u1);
        i2 = convertToDecimal(u2);

        val = i1 - i2;

        if (val < 0)
            val *= -1;

        return convertToUnary(val);
    }

    // }}}
    // {{{ power()
    /**
     * Returns u1 to the power of u2.
	 *
     * @param base
     * @param exp
     */
    public static UnaryNumber power(UnaryNumber base, UnaryNumber exp)
    {
        int iexp;
        int val;
        int ibase;

		// base of 0: return 0
        if (base.getValue() == "")
            return base;

		// exponent of 0: return 1
        if (exp.getValue() == "")
            return convertToUnary(1);

        iexp = convertToDecimal(exp);
        ibase = convertToDecimal(base);

        val = 1;

        while (iexp-- > 0) {
            val *= ibase;
        }

        return convertToUnary(val);
    }

    // }}}
    // {{{ convertToUnary()
    /**
     * Converts the given integer to a UnaryNumber.
	 *
     * @param  n Number as <code>int</code>
     * @return   <code>UnaryNumber</code> representation
     */
    public static UnaryNumber convertToUnary(int n)
    {
        String s = new String("");

        while (n-- > 0)
            s += "I";

        return new UnaryNumber(s);
    }

    // }}}
    // {{{ convertToDecimal()
    /**
     * Converts the given UnaryNumber to an integer.
	 *
     * @param  u <code>UnaryNumber</code> representation
     * @return   <code>int</code> representation
     */
    public static int convertToDecimal(UnaryNumber u)
    {
        return u.getValue().length();
    }

    // }}}
}
