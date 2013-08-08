import java.util.LinkedList;
import java.util.Iterator;

public class Vertex
{
	private String label;
	protected Vertex parent;
	protected LinkedList children;
	private int distance = -1;
	protected int orderLabel = -1;

	public Vertex (String label, Vertex parent)
	{
		this.label = label;
		this.parent = parent;
		this.children = new LinkedList ();

		if (parent != null)
		{
			parent.children.add (this);
		}
	}

	/*
	 * Returns the height of the tree rooted at this Vertex
	 *
	 * A tree with 1 Vertex has height 1 -- 2 Vertices has height 2.
	 * Any more will be >= 2
	 */
	public int getHeight ()
	{
		Iterator i;
		LinkedList curList;
		LinkedList nextList;
		Vertex v;
		int height = 0;

		curList = new LinkedList ();
		curList.add (this);

		while (true)
		{
			height++;

			nextList = new LinkedList ();

			for (i = curList.iterator (); i.hasNext (); )
			{
				v = (Vertex) i.next ();

				nextList.addAll (v.children);
			}

			if (nextList.isEmpty ()) break;

			curList = nextList;
		}

		return height;
	}

	/*
	 * Returns the depth of the Vertex
	 *
	 * The root Vertex has depth 0. Any lower will be >= 1
	 */
	public int getDepth ()
	{
		int depth = 0;
		Vertex v = this;

		while (v.parent != null)
		{
			v = v.parent;
			depth++;
		}

		return depth;
	}

	/*
	 * Implements first half of RootedTree.calculateDistanceFromLeaves ()
	 *
	 * calculates the distance from this Vertex to the closest null below
	 */
	protected void calculateDistanceFromLeaves_children ()
	{
		Vertex child;
		int t;

		distance = 0;

		if (children.isEmpty ())
		{
			return;
		}

		// Calculate (minimum distance of a child) + 1
		for (Iterator i = children.iterator (); i.hasNext ();)
		{
			child = (Vertex) i.next ();

			child.calculateDistanceFromLeaves_children ();

			t = child.distance + 1;

			if (distance == 0 || t < distance)
			{
				distance = t;
			}
		}
	}

	/*
	 * Implements second half of RootedTree.calculateDistanceFromLeaves ()
	 *
	 * If the parent is closer to null than children, recalculate children
	 */
	protected void calculateDistanceFromLeaves_parent ()
	{
		Vertex child;
		int t;

		if (parent == null && children.isEmpty ())
		{
			distance = 0;
		}

		if (parent != null)
		{
			t = parent.distance + 1;

			if (t < distance)
			{
				distance = t;
			}
		}

		for (Iterator i = children.iterator (); i.hasNext (); )
		{
			child = (Vertex) i.next ();

			child.calculateDistanceFromLeaves_parent ();
		}
	}

	/*
	 * Returns a string representing the rooted tree at this Vertex
	 */
	public String toString ()
	{
		Iterator i;
		Vertex v;
		String ret;

		ret = getLabel ();

		for (i = children.iterator (); i.hasNext (); )
		{
			v = (Vertex) i.next ();
			ret += "(" + v.toString () + ")";
		}

		return ret;
	}

	protected int getDistance ()
	{
		return distance;
	}

	public String getLabel ()
	{
		return this.label;
	}
}
