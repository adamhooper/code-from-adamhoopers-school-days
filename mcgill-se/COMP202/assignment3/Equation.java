//Name: Adam Hooper
//McGIll ID: 260055737
//Course: COMP202
//Section: 002
//Assignment: 3
//Developed at: home
//
//Equation: given a string of digits, will output an equation using them

import cs1.Keyboard;
import java.util.LinkedList;
import java.util.ListIterator;
import java.util.Hashtable;
import java.util.StringTokenizer;

// {{{ EquationSolver
/**
 * Prints a combination of digits and +-*%/= to create an equation from digits.
 *
 * <p>How it works:</p>
 * <ol>
 * <li>Store the given equation.</li>
 * <li>Put the '=' operator halfway between the digits.</li>
 * <li>Calculate a hash mapping each possible value of the left-hand side of
 *     the equation to any equation giving that value.</li>
 * <li>Calculate a list of all possible equations on the right-hand side.</li>
 * <li>Loop through the equations in (4). Calculate the value each time. If a
 *     key in the hash from (3) is found, return the left-hand combination +
 *     <samp>=</samp> + the right-hand combination.</li>
 * <li>Move the '=' sign somewhere else (close to the middle, for efficiency)
 *     If '=' has already been tried in every location, admit defeat.</li>
 * <li>Start again at (3).</li>
 * </ol>
 * <p>Rationale:</p>
 * <ul>
 * <li>One side uses a hash and another does not. This is kind of strange, but
 *     not without reason! The hash only stores the equations which are useful,
 *     whereas the list stores ALL equations. The hash calculations have all
 *     been done in advance, whereas the list ones haven't. Since we check
 *     against the hash several times, this is faster. The right-hand side
 *     isn't converted to a hash because it's probable we won't even have to
 *     calculate half the values at all.</li>
 * <li>I used LinkedLists and hashes because I'm used to C and PHP ;).</li>
 * <li>I used Javadoc for the hell of it.</li>
 * </ul>
 */
class EquationSolver
{
    private String digits;

    // {{{ constructor
    /**
     * Creates a new EquationSolver.
	 *
     * @param d Digits with which to create an equation
     */
    public EquationSolver(String d)
    {
        digits = d;
    }

    // }}}
    // {{{ findEquation()
    /**
     * Puts =,+,-,*,/ in between digits to make a valid equation.
	 *
     * @param  digits A string of digits from 0 to 9
     * @return        Equation, or empty string if none could be found
     */
    public String findEquation()
    {
        int i;
		int[] equalOrder;
        String lhs, rhs;
        String equation;

        if (digits.length() < 2)
            return "";

		// This is much more efficient than putting the '=' way to the left
		equalOrder = equalPlacementOrder();

		for (i = 0; i < equalOrder.length; i++) {
			lhs = digits.substring(0, equalOrder[i]);
			rhs = digits.substring(equalOrder[i]);

			equation = matchLhsRhs(lhs, rhs);
			if (equation.length() > 0) {
				return equation;
			}
		}

        return "";
    }

    // }}}
	// {{{ equalPlacementOrder()
	/**
	 * Returns an array of preferred locations of '=' character.
	 * For efficiency, we'll try to put the '=' as close to the middle
	 * as possible (to reduce permutations). For example, in "1234" we'll
	 * first try (12)=(34) (where the brackets mean we'll put in operations).
	 * Next, try (123)=(4) and then (1)=(234).
	 *
	 * @return An array wherein the first element is the preferred '=' sign
	 *         placement, followed by the second-best, etc.
	 */
	private int[] equalPlacementOrder()
	{
		char nextOper = '-';
		int[] ret = new int[digits.length()-1];
		int cur;
		int i;

		cur = digits.length() / 2;

		for (i = 0; i < digits.length() - 1; i++) {
			switch (nextOper) {
				case '+':
					cur += i;
					nextOper = '-';
					break;
				case '-':
					cur -= i;
					nextOper = '+';
					break;
			}

			ret[i] = cur;
		}

		return ret;
	}

	// }}}
    // {{{ matchLhsRhs()
    /**
     * Returns an equation with (lhs combination) = (rhs combination).
	 *
     * @param  lhs Left-hand digits
     * @param  rhs Right-hand digits
     * @return     Equation with =,+,-,*,/ -- or empty string if impossible
     */
    private String matchLhsRhs(String lhs, String rhs)
    {
		String cur;
		Integer curValue;

		Hashtable hashLhs = hashCombinations(listCombinations(lhs));
		LinkedList listRhs = listCombinations(rhs);
		ListIterator li = listRhs.listIterator(0);

		while (li.hasNext()) {
			cur = li.next().toString();
			curValue = new Integer(getValue(cur));

			if (hashLhs.containsKey(curValue))
				return hashLhs.get(curValue) + "=" + cur;
		}

		return "";
    }

    // }}}
    // {{{ listCombinations()
    /**
     * Returns a list of all possible combinations of digits and operations.
     * <p>For example, listNumbers(12) will return a list with:</p>
	 * <ul>
	 *  <li><samp>12</samp></li>
	 *  <li><samp>1+2</samp></li>
	 *  <li><samp>1-2</samp></li>
	 *  <li><samp>1*2</samp></li>
	 *  <li><samp>1/2</samp></li>
	 *  <li><samp>1%2</samp></li>
	 * </ul>
     * <p>...where each of the preceding bullets is a <code>String</code></p>
	 *
	 * <p>This is a recursive function.</p>
     *
     * @param  digits String of digits
     * @return        List of combinations
     */
    private LinkedList listCombinations(String digits)
    {
		int i;
		LinkedList ret = new LinkedList();

		if (digits.length() == 1) {
			ret.add(digits);
			return ret;
		}

		ret = listCombinations(digits.substring(1));

		ret = permute(digits.charAt(0), ret);

		return ret;
	}

    // }}}
	// {{{ permute()
	/**
	 * Puts the given <code>char</code> in the beginning of the
	 * <code>LinkedList</code> with operators.
	 *
	 * <p>For example, given the <code>char<code> <samp>2</samp> and a
	 * <code>LinkedList</code> with <code>String</code>s <samp>34</samp> and
	 * <samp>3+4</samp>, returns a <code>LinkedList</code> with
	 * <code>String</code>s (<samp>234</samp>, <samp>2+34</samp>, ...,
	 * <samp>2+3+4</samp>, <samp>2*3+4</samp>, and so on)</p>
	 *
	 * @param firstDigit   The digit to prepend in various ways
	 *        combinations The combinations to which we prepend the digit
	 * @return             A new <code>LinkedList</code> like combinations
	 */
	private LinkedList permute(char firstDigit, LinkedList combinations)
	{
		LinkedList ret = new LinkedList();
		ListIterator li = combinations.listIterator(0);
		String cur;

		while (li.hasNext()) {
			cur = li.next().toString();

			ret.add(new String(firstDigit + cur));
			ret.add(new String(firstDigit + "+" + cur));
			ret.add(new String(firstDigit + "*" + cur));
			ret.add(new String(firstDigit + "-" + cur));

			if (!isFirstTermZero(cur)) {
				ret.add(new String(firstDigit + "/" + cur));
				ret.add(new String(firstDigit + "%" + cur));
			}
		}

		return ret;
	}

	// }}}
	// {{{ getValue()
	/**
	 * Returns an <code>int</code> from a <code>String</code> combination of
	 * digits and operators.
	 *
	 * @param  s Combination of digits and operators, for example
	 *           <samp>12+3</samp>
	 * @return   Value of s: for example, <samp>15</samp>
	 */
	private int getValue(String s)
	{
		char oper;
		int left, right;

		StringTokenizer st = new StringTokenizer(s, "+-*/%", true);

		left = Integer.parseInt(st.nextToken());

		while (st.hasMoreTokens()) {
			oper = st.nextToken().charAt(0);
			right = Integer.parseInt(st.nextToken());

			switch (oper) {
				case '+':
					left += right;
					break;
				case '-':
					left -= right;
					break;
				case '/':
					left /= right;
					break;
				case '*':
					left *= right;
					break;
				case '%':
					left %= right;
					break;
				default:
					System.err.println("Invalid character in getValue(): "
									   + oper);
					System.exit(1);
			}
		}

		return left;
	}

	// }}}
	// {{{ isFirstTermZero()
	/**
	 * Returns whether the first set of digits in the given <samp>String</samp>
	 * is zero.
	 * <p>For example, the following will return <code>true</code>:
	 * <samp>00+3</samp></p>
	 * <p>This will return <code>false</code>: <samp>01+3</samp></p>
	 *
	 * @param  s <code>String</code> starting with digits
	 * @return   Whether the first set of digits is all <samp>0</samp>
	 */
	private boolean isFirstTermZero(String s)
	{
		int i;
		char c;

		for (i = 0; i < s.length(); i++) {
			c = s.charAt(i);

			if (c != '0') {
				if (Character.isDigit(c))
					return false;
				else
					return true;
			}
		}

		return true;
	}

	// }}}
	// {{{ hashCombinations()
    /**
     * From a given <code>LinkedList</code> of <code>String</code>s, gives
	 * <code>Hashtable</code> of value -&gt; combination.
     *
     * @param  combinations List of <code>String</code>s of digits and
	 *                      operations
     * @return              <code>HashTable</code> mapping <code>int</code>
	 *                      value to <code>String</code> combination
     */
    private Hashtable hashCombinations(LinkedList combinations)
    {
		Hashtable ret = new Hashtable();

		String combination;
		Integer value;
		ListIterator li = combinations.listIterator(0);

		while (li.hasNext()) {
			combination = li.next().toString();
			value = new Integer(getValue(combination));

			if (!ret.containsKey(value))
				ret.put(value, combination);
		}

		return ret;
    }

    // }}}
}

// }}}
// {{{ Equation
public class Equation
{
    // {{{ main()
    /**
     * Asks for a string of digits and prints an equation with them.
     */
    public static void main(String[] argv)
    {
        String equation;
		String input;
        EquationSolver solver;

		System.out.print("Please enter a numeric string: ");
		input = Keyboard.readString();

		solver = new EquationSolver(input);

        equation = solver.findEquation();

        if (equation.length() == 0) {
            System.out.println("Equation could not be found");
        } else {
			System.out.print("An equation was found: ");
            System.out.println(equation);
        }
    }

    // }}}
}

// }}}

// vim: set sw=4 ts=4 noet:
// vim600: set foldmethod=marker:
