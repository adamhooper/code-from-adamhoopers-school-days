import java.util.LinkedList;
import java.util.Iterator;

public class Map
{
	private LinkedList map = new LinkedList ();

	/*
	 * Declare 2 Vertices to be isomorphic
	 */
	public void add (Vertex a, Vertex b)
	{
		Vertex[] n = new Vertex[2];
		n[0] = a;
		n[1] = b;

		map.add (n);
	}

	/*
	 * Return a string list of all isomorphic relations
	 */
	public String toString ()
	{
		String r = null;
		Vertex[] entry;
		Vertex a;
		Vertex b;

		for (Iterator i = map.iterator (); i.hasNext (); )
		{
			entry = (Vertex[]) i.next ();
			a = entry[0];
			b = entry[1];

			if (r == null)
			{
				r = "{";
			}
			else
			{
				r += ", ";
			}

			r += "(" + a.getLabel () + ", " + b.getLabel () + ")";
		}

		r += "}";

		return r;
	}
}
