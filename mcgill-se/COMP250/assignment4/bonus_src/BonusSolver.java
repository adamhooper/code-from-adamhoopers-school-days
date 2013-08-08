import java.util.Stack;
import java.util.Iterator;
import java.util.TreeSet;

public class BonusSolver
{
	private Stack cur_list;
	private TreeSet next_list;
	private int weight;
	private byte[][] moves;

	public BonusSolver (int weight)
	{
		cur_list = new Stack ();
		cur_list.ensureCapacity (3626632);
		next_list = new TreeSet ();
		this.weight = weight;
		moves = alloc_moves ();
	}

	public byte[][] alloc_moves ()
	{
		// There CAN NEVER be more than, say, 50 possible moves

		byte[][] ret;

		ret = new byte[50][3];

		return ret;
	}

	public void finalizeStep ()
	{
		Iterator next_i;

		for (next_i = next_list.iterator (); next_i.hasNext ();)
		{
			cur_list.push (next_i.next ());
		}

		next_list.clear ();
	}

	public void step ()
	{
		MyHiQ cur;
		MyHiQ next;
		MyHiQ existing;
		byte[] move;
		int move_i;
		boolean added;

		while (cur_list.isEmpty () == false)
		{
			cur = (MyHiQ) cur_list.pop ();

			cur.findMoves (moves);

			for (move_i = 0; moves[move_i][0] != -1; move_i++)
			{
				next = clone (cur);

				next.move (moves[move_i]);

				added = next_list.add (next);

				if (added == false)
				{
					existing = (MyHiQ) next_list.tailSet (next).first ();

					existing.num_variations += next.num_variations;
				}
			}
		}

		finalizeStep ();

		weight--;
	}

	public void printState ()
	{
		MyHiQ hiq;
		Iterator next_i;
		int i = 0;

		System.out.println ("PRINTING STATE at weight " + weight);

		for (next_i = cur_list.iterator (); next_i.hasNext (); i++)
		{
			hiq = (MyHiQ) next_i.next ();
			hiq.print ();
		}

		System.out.println ("DONE PRINTING. Total " + i + " configs");
	}

	public void solve ()
	{
		MyHiQ hiq;
		long startTime, endTime;

		hiq = new MyHiQ (weight);

		cur_list.add (hiq);

		startTime = System.currentTimeMillis ();

		while (weight > 1)
		{
			step ();
			endTime = System.currentTimeMillis ();

			System.out.println ("Weight " + weight + ". We're at " + (endTime - startTime) + "ms");

			System.out.println ("   " + cur_list.size() + " unique configurations");
		}

		printState ();
	}

	public MyHiQ clone (MyHiQ hiq)
	{
		MyHiQ ret = new MyHiQ ();
		ret.config = hiq.config;
		ret.num_variations = hiq.num_variations;
		return ret;
	}
}
