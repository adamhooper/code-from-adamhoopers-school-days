// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP-202
// Section: 002
// Assignment: 4
// Developed at: home
//
// Rectangle: represents a Rectangle in 2D space

public class Rectangle
{
	Point tl, br;

	// {{{ constructor
	/**
	 * Creates a new <code>Rectangle</code> from the two given
	 * <code>Point</code>s.
	 *
	 * @param p1 Top-left corner point
	 * @param p2 Bottom-right corner point
	 */
	public Rectangle(Point p1, Point p2)
	{
		this.tl = p1;
		this.br = p2;
	}

	// }}}
	// {{{ get_top_left()
	public Point get_top_left()
	{
		return tl;
	}

	// }}}
	// {{{ get_top_right()
	public Point get_top_right()
	{
		return new Point(br.get_x(), tl.get_y());
	}

	// }}}
	// {{{ get_bottom_left()
	public Point get_bottom_left()
	{
		return new Point(tl.get_x(), br.get_y());
	}

	// }}}
	// {{{ get_bottom_right()
	public Point get_bottom_right()
	{
		return this.br;
	}

	// }}}
	// {{{ print()
	/**
	 * Prints the corner points of the rectangle in clockwise order (tl, tr,
	 * br, bl): for example, '[(3,4),(5,4),(5,-3),(3,-3)]'.
	 */
	public void print()
	{
		Point tr = this.get_top_right();
		Point bl = this.get_bottom_left();

		System.out.print("[");
		tl.print();
		System.out.print(",");
		tr.print();
		System.out.print(",");
		br.print();
		System.out.print(",");
		bl.print();
		System.out.println("]");
	}

	// }}}
	// {{{ area()
	/**
	 * Returns this <code>Rectangle</code>'s area.
	 *
	 * @return Area
	 */
	public double area()
	{
		double x = br.get_x() - tl.get_x();
		double y = tl.get_y() - br.get_y();

		return x * y;
	}

	// }}}
	// {{{ translate()
	/**
	 * Moves this object by the given amounts.
	 *
	 * @param dx Shift in x-axis
	 * @param dy Shift in y-axis
	 */
	public void translate(double dx, double dy)
	{
		tl.translate(dx, dy);
		br.translate(dx, dy);
	}

	/**
	 * Moves this object so that the top-left is on the given
	 * <code>Point</code>.
	 *
	 * @param pt Point which should be the new top-left point
	 */
	public void translate(Point pt)
	{
		double dx = pt.get_x() - tl.get_x();
		double dy = pt.get_y() - tl.get_y();

		translate(dx, dy);
	}

	// }}}
	// {{{ equals()
	/**
	 * Returns whether the given <code>Rectangle</code> is the same as this one.
	 *
	 * @param  r Rectangle for comparison
	 * @return   Equality
	 */
	public boolean equals(Rectangle r)
	{
		return (tl.equals(r.get_top_left()) && br.equals(r.get_bottom_right()));
	}

	// }}}
	// {{{ same_as()
	/**
	 * Return whether the given <code>Rectangle</code> is a reference to this
	 * one.
	 *
	 * @param  r <code>Rectangle</code> for comparison.
	 * @return   Exact equality
	 */
	public boolean same_as(Rectangle r)
	{
		return this == r;
	}

	// }}}
	// {{{ smaller_than()
	/**
	 * Returns <code>true</code> if the area of the given <code>Rectangle</code>
	 * is greater than this one.
	 *
	 * @param r <code>Rectangle</code> to compare.
	 * @return  Whether this one is smaller.
	 */
	public boolean smaller_than(Rectangle r)
	{
		return this.area() > r.area();
	}

	// }}}
	// {{{ contained_in()
	/**
	 * Returns <code>true</code> if this <code>Rectangle</code> is inside the
	 * given one.
	 *
	 * @param r <code>Rectangle</code> to compare.
	 * @return  Whether this one is inside r.
	 */
	public boolean contained_in(Rectangle r)
	{
		return (tl.get_x() >= r.get_top_left().get_x() &&
				br.get_x() <= r.get_bottom_right().get_x() &&
				tl.get_y() <= r.get_top_left().get_y() &&
				br.get_y() >= r.get_bottom_right().get_y());
	}

	// }}}
	// {{{ contains()
	/**
	 * Returns <code>true</code> if the given <code>Rectangle</code> is inside
	 * this one.
	 *
	 * @param r <code>Rectangle</code> to compare.
	 * @return  Whether r is inside this one.
	 */
	public boolean contains(Rectangle r)
	{
		return r.contained_in(this);
	}

	/**
	 * Returns <code>true</code> if the given <code>Point</code> is inside this
	 * <code>Rectangle</code>.
	 *
	 * @param pt <code>Point</code> in 2D space.
	 * @return   Whether pt is inside this <code>Rectangle</code>.
	 */
	public boolean contains(Point pt)
	{
		return (tl.get_x() <= pt.get_x() && br.get_x() >= pt.get_x() &&
				tl.get_y() >= pt.get_y() && br.get_y() <= pt.get_y());
	}

	// }}}
}
