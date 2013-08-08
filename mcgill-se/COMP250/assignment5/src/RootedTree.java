import java.util.LinkedList;
import java.util.Iterator;
import java.util.Comparator;

public class RootedTree
{
	private LinkedList vertices;
	private Vertex root;

	public RootedTree (String representation)
	{
		vertices = new LinkedList ();
		buildFromRepresentation (representation);
	}

	/*
	 * Populates this RootedTree from a given string
	 * e.g., "a(b(c))(d(e)(f))" -->
	 *           a
	 *          / \
	 *         b   d
	 *        /   / \
	 *       c   e   f
	 */
	private void buildFromRepresentation (String repr)
	{
		int nextLeftParen;
		int nextRightParen;
		int nextParen;
		Vertex parent = null;
		Vertex child = null;

		while (repr.length () > 0)
		{
			nextLeftParen = repr.indexOf ('(');
			nextRightParen = repr.indexOf (')');

			if (nextLeftParen >= 0
			    && nextLeftParen < nextRightParen)
			{
				nextParen = nextLeftParen;
			}
			else
			{
				nextParen = nextRightParen;
			}

			// handle single-node tree
			if (nextParen == -1)
			{
				root = new Vertex (repr, null);
				vertices.add (root);
				return;
			}

			if (nextParen != 0)
			{
				child = new Vertex
					(repr.substring (0, nextParen),
					 parent);

				vertices.add (child);
			}

			if (nextParen == nextLeftParen)
			{
				// Go down a level
				parent = child;
				child = null;
			}
			else // nextParen == nextRightParen
			{
				// Return up a level
				child = parent;
				parent = parent.parent;
			}

			repr = repr.substring (nextParen + 1);
		}

		root = child;
	}

	public String toString ()
	{
		return root.toString ();
	}

	/*
	 * Populates all vertices' "distance"
	 */
	private void calculateDistanceFromLeaves ()
	{
		if (root == null) return;

		root.calculateDistanceFromLeaves_children ();
		root.calculateDistanceFromLeaves_parent ();
	}

	/*
	 * Returns 1 or 2 center vertices (farthest from any edge)
	 */
	private Vertex[] findCenter ()
	{
		Iterator i;
		Vertex v;
		Vertex v1 = null;
		Vertex v2 = null;
		Vertex[] ret;
		int maxDistance = -1;

		if (root == null) return null;

		calculateDistanceFromLeaves ();

		for (i = vertices.iterator (); i.hasNext ();)
		{
			v = (Vertex) i.next ();

			if (v.getDistance () == maxDistance)
			{
				v2 = v;
			}

			if (v.getDistance () > maxDistance)
			{
				maxDistance = v.getDistance ();
				v1 = v;
			}
		}

		if (v1 != null && v2 != null
		    && v1.getDistance () == v2.getDistance ()) // two root nodes
		{
			ret = new Vertex[2];
			ret[0] = v1;
			ret[1] = v2;
		}
		else
		{
			ret = new Vertex[1];
			ret[0] = v1;
		}

		return ret;
	}

	/*
	 * Makes v the root of this tree
	 */
	private void reRoot (Vertex v)
	{
		Vertex p, g;

		if (v == null) return;

		root = v;

		p = v.parent;
		v.parent = null;

		while (p != null)
		{
			p.children.remove (v);
			v.children.add (p);

			g = p.parent;
			p.parent = v;

			v = p;
			p = g;
		}
	}

	/*
	 * Zeroes all vertices' order labels
	 */
	private void initializeVertices ()
	{
		Iterator i;
		Vertex v;

		for (i = vertices.iterator (); i.hasNext (); )
		{
			v = (Vertex) i.next ();

			v.orderLabel = 0;
		}
	}

	/*
	 * Returns a LinkedList of all vertices at depth d
	 */
	private LinkedList verticesAtDepth (int d)
	{
		Iterator i;
		Vertex v;
		LinkedList curList, nextList;

		curList = new LinkedList ();
		curList.add (root);

		while (d-- > 0)
		{
			nextList = new LinkedList ();

			for (i = curList.iterator (); i.hasNext (); )
			{
				v = (Vertex) i.next ();

				nextList.addAll (v.children);
			}

			curList = nextList;
		}

		return curList;
	}

	/*
	 * Sets the order labels on all vertices at the given depth.
	 * Returns the vertices at that depth
	 *
	 * This function also rearranges the vertices below this depth
	 */
	private LinkedList labelLevel (int depth)
	{
		LinkedList ret;
		Iterator i;
		Vertex v;
		Vertex prevV;
		int newLabel;
		Vertex[] sortVertices;
		Comparator comp;

		ret = verticesAtDepth (depth);

		// sort the children of every vertex at this depth
		comp = new ComparatorByVertexOrderLabels ();

		for (i = ret.iterator (); i.hasNext (); )
		{
			v = (Vertex) i.next ();

			Sorter.sort (v.children, comp);
		}

		// sort the whole list at this depth
		comp = new ComparatorByVertexChildOrderLabels ();

		Sorter.sort (ret, comp);

		// label these ones
		v = null;
		newLabel = -1;
		for (i = ret.iterator (); i.hasNext (); )
		{
			prevV = v;
			v = (Vertex) i.next ();

			if (prevV == null || comp.compare (prevV, v) != 0)
			{
				// v > prevV
				v.orderLabel = ++newLabel;
			}
			else
			{
				// v == prevV
				v.orderLabel = newLabel;
			}
		}

		return ret;
	}

	static class ComparatorByVertexOrderLabels implements Comparator {
		public int compare (Object a, Object b)
		{
			return ((Vertex) a).orderLabel
				- ((Vertex) b).orderLabel;
		}
	}

	/*
	 * Compares two vertices by the lists of their children's order labels
	 *
	 * Vertices' lists of children must already be in order
	 */
	static class ComparatorByVertexChildOrderLabels implements Comparator {
		public int compare (Object a, Object b)
		{
			LinkedList aChildren = ((Vertex) a).children;
			LinkedList bChildren = ((Vertex) b).children;
			Iterator ai, bi;
			Vertex av, bv;
			int diff;

			diff = aChildren.size () - bChildren.size ();
			if (diff != 0) return diff;

			for (ai = aChildren.iterator (),
			      bi = bChildren.iterator ();
			     ai.hasNext () && bi.hasNext (); )
			{
				av = (Vertex) ai.next ();
				bv = (Vertex) bi.next ();

				diff = av.orderLabel - bv.orderLabel;

				if (diff != 0) return diff;
			}

			return 0;
		}
	}

	/*
	 * Returns a Map if the given rooted trees are isomorphic
	 */
	static private Map rootedIsomorphic (RootedTree t1, RootedTree t2)
	{
		LinkedList L1, L2;
		Iterator i1, i2;
		Vertex v1, v2;
		Comparator comp = new ComparatorByVertexChildOrderLabels ();
		Map ret;

		int h = t1.root.getHeight ();

		if (h != t2.root.getHeight ()) return null;

		t1.initializeVertices ();
		t2.initializeVertices ();

		L1 = t1.verticesAtDepth (h - 1);
		L2 = t2.verticesAtDepth (h - 1);

		for (int depth = h - 2; depth >= 0; depth--)
		{
			L1 = t1.labelLevel (depth);
			L2 = t2.labelLevel (depth);

			if (L1.size () != L2.size ())
			{
				return null;
			}

			for (i1 = L1.iterator (), i2 = L2.iterator ();
			     i1.hasNext () && i2.hasNext (); )
			{
				v1 = (Vertex) i1.next ();
				v2 = (Vertex) i2.next ();

				if (comp.compare (v1, v2) != 0)
				{
					return null;
				}
			}
		}

		ret = new Map ();
		generateMapping (t1.root, t2.root, ret);
		return ret;
	}

	/*
	 * Recursively constructs Map m, given that v1 and v2 are isomorphic
	 */
	static private void generateMapping (Vertex v1, Vertex v2, Map m)
	{
		Iterator i1, i2;
		Vertex child1, child2;

		m.add (v1, v2);

		for (i1 = v1.children.iterator (),
		      i2 = v2.children.iterator ();
		     i1.hasNext () && i2.hasNext (); )
		{
			child1 = (Vertex) i1.next ();
			child2 = (Vertex) i2.next ();

			generateMapping (child1, child2, m);
		}
	}

	/*
	 * Returns a Map if t1 and t2 are isomorphic
	 *
	 * This will re-root the trees if necessary, then call isomorphic()
	 */
	static Map isomorphic (RootedTree t1, RootedTree t2)
	{
		Map ret;
		Vertex[] c1, c2; // Arrays of at most 2 vertices each

		c1 = t1.findCenter ();
		t1.reRoot (c1[0]);
		c2 = t2.findCenter ();
		t2.reRoot (c2[0]);

		ret = rootedIsomorphic (t1, t2);

		if (ret == null && c1.length == 2)
		{
			t1.reRoot (c1[1]);

			ret = rootedIsomorphic (t1, t2);
		}

		return ret;
	}
}
