// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP-202
// Section: 002
// Assignment: 4
// Developed at: home
//
// Point: represents a point in 2D space

public class Point
{
	double x, y;

	// {{{ constructor
	public Point(double x, double y)
	{
		this.x = x;
		this.y = y;
	}

	// }}}
	// {{{ get_x()
	/**
	 * Returns the value of this <code>Point</code>'s x coordinate.
	 *
	 * @return The x value
	 */
	public double get_x()
	{
		return this.x;
	}

	// }}}
	// {{{ get_y()
	/**
	 * Returns the value of this <code>Point</code>'s y coordinate.
	 *
	 * @return The y value
	 */
	public double get_y()
	{
		return this.y;
	}

	// }}}
	// {{{ print()
	/**
	 * Prints "(x,y)", where x and y are this point's coordinates.
	 */
	public void print()
	{
		System.out.print("(" + x + "," + y + ")");
	}

	// }}}
	// {{{ translate()
	/**
	 * Shifts this <code>Point</code> by the given values.
	 *
	 * @param dx Shift along the x-axis
	 * @param dy Shift along the y-axis
	 */
	public void translate(double dx, double dy)
	{
		this.x += dx;
		this.y += dy;
	}

	// }}}
	// {{{ equals()
	/**
	 * Returns whether or not the given <code>Point</code> is the same one.
	 *
	 * @param pt The point to test for equality
	 * @return   Equality
	 */
	public boolean equals(Point pt)
	{
		return (pt.x == this.x && pt.y == this.y);
	}

	// }}}
	// {{{ same_as()
	/**
	 * Returns whether the given <code>Point</code> is <em>exactly</em> the
	 * same as this one (that is, same memory location).
	 *
	 * @param pt Point to test for exact equality
	 * @return   Exact equality
	 */
	public boolean same_as(Point pt)
	{
		return this == pt;
	}

	// }}}
}
