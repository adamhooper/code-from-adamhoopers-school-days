import java.util.TreeSet;

public class Solver
{
	private MyHiQ start;
	private MyHiQ end;
	private Factory factory;
	private byte[][][] move_cache;
	private TreeSet cache;

	public Solver ()
	{
		end = new MyHiQ (1);
		start = new MyHiQ (32);
		factory = new Factory ();
		cache = new TreeSet ();

		move_cache = new byte[32][50][3];
	}

	public void solve ()
	{
		StringBuffer path = new StringBuffer ();

		step (start, 32, path);

		System.out.println (path);
	}

	private boolean step (MyHiQ hiq, int weight, StringBuffer path)
	{
		int i;
		MyHiQ t;
		byte[][] moves = move_cache[weight - 1];
		Long hash;
		boolean unchecked;

		if (weight == 1)
		{
			return hiq.equals (end);
		}

		hiq.findMoves (moves);

		for (i = 0; moves[i][0] != -1; i++)
		{
			t = factory.clone (hiq);
			t.move (moves[i]);

			hash = new Long (t.hash ());

			unchecked = cache.add (hash);

			if (unchecked == false)
			{
				return false;
			}

			if (step (t, weight - 1, path))
			{
				path.insert (0, moves[i][0] + "->"
						+ moves[i][2]);

				if (weight != 32)
				{
					path.insert (0, ", ");
				}

				return true;
			}

			factory.recycle (t);
		}

		return false;
	}
}
