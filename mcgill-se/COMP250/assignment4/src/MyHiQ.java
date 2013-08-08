public class MyHiQ
	implements Comparable
{
	/*
	 * config:
	 * [................................................................]
	 *  |<-------------------------->| (63-33) unused
	 * (32-0) board configuration     |<------------------------------>|
	 */ 
	public long config;
	private static long alt_configs[]; // cache -- avoid GC

	private final static byte[][] rows =
	{
		{ 0, 1, 2 },
		{ 3, 4, 5 },
		{ 6, 7, 8, 9, 10, 11, 12 },
		{ 13, 14, 15, 16, 17, 18, 19 },
		{ 20, 21, 22, 23, 24, 25, 26 },
		{ 27, 28, 29 },
		{ 30, 31, 32 }
	};
	private final static byte[][] cols =
	{
		{ 6, 13, 20 },
		{ 7, 14, 21 },
		{ 0, 3, 8, 15, 22, 27, 30 },
		{ 1, 4, 9, 16, 23, 28, 31 },
		{ 2, 5, 10, 17, 24, 29, 32 },
		{ 11, 18, 25 },
		{ 12, 19, 26 }
	};

	public MyHiQ (int n)
	{
		if (alt_configs == null) alt_configs = new long[8];
		if (n == 1)
		{
			config = 1L << 16;
		}
		else if (n == 4)
		{
			config = 1L << 8
				 | 1L << 9
				 | 1L << 15
				 | 1L << 16;
		}
		else if (n == 10)
		{
			config = 8807832L;
		}
		else if (n == 20)
		{
			config = 6348530494L;
		}
		else
		{
			config = 8589869055L;
		}
		config |= 1L << 33;
	}

	public MyHiQ ()
	{
	}

	public void findMoves (byte[][] moves)
	{
		int i, j;
		int move_num = 0;

		for (i = 0; i < rows.length; i++)
		{
			for (j = 0; j < rows[i].length - 2; j++)
			{
				// left to right
				if ((config & 1L << rows[i][j]) > 0
				    && (config & 1L << rows[i][j + 1]) > 0
				    && (config & 1L << rows[i][j + 2]) == 0)
				{
					moves[move_num][0] = rows[i][j];
					moves[move_num][1] = rows[i][j + 1];
					moves[move_num][2] = rows[i][j + 2];

					move_num++;
				}

				// right to left
				if ((config & 1L << rows[i][j + 2]) > 0
				    && (config & 1L << rows[i][j + 1]) > 0
				    && (config & 1L << rows[i][j]) == 0)
				{
					moves[move_num][0] = rows[i][j + 2];
					moves[move_num][1] = rows[i][j + 1];
					moves[move_num][2] = rows[i][j];

					move_num++;
				}

				// top to bottom
				if ((config & 1L << cols[i][j]) > 0
				    && (config & 1L << cols[i][j + 1]) > 0
				    && (config & 1L << cols[i][j + 2]) == 0)
				{
					moves[move_num][0] = cols[i][j];
					moves[move_num][1] = cols[i][j + 1];
					moves[move_num][2] = cols[i][j + 2];

					move_num++;
				}

				// bottom to top
				if ((config & 1L << cols[i][j + 2]) > 0
				    && (config & 1L << cols[i][j + 1]) > 0
				    && (config & 1L << cols[i][j]) == 0)
				{
					moves[move_num][0] = cols[i][j + 2];
					moves[move_num][1] = cols[i][j + 1];
					moves[move_num][2] = cols[i][j];

					move_num++;
				}
			}
		}

		moves[move_num][0] = -1;
	}

	public void move (byte[] move)
	{
		config ^= 1L << move[0];
		config ^= 1L << move[1];
		config |= 1L << move[2];
	}

	public void print ()
	{
		int i, j;
		char c;

		for (i = 0; i < rows.length; i++)
		{
			if (rows[i].length == 3)
			{
				System.out.print ("  ");
			}

			for (j = 0; j < rows[i].length; j++)
			{
				if ((config & 1L << rows[i][j]) != 0)
				{
					c = '⊗';
				}
				else
				{
					c = '·';
				}

				System.out.print ("" + c);
			}

			System.out.println ();
		}

		for (i = 63; i >= 0; i--)
		{
			System.out.print ((config & 1L << i) > 0 ? 1 : 0);
		}
		System.out.println ();
	}

	private long max (long[] a)
	{
		long max = a[0];

		for (int i = 1; i < a.length; i++)
		{
			if (max < a[i])
			{
				max = a[i];
			}
		}

		return max;
	}

	public long hash ()
	{
		int i, j;
		int rev_i, rev_j; // "reverse": i/j counted from right to left

		alt_configs[0] = config;

		for (i = 1; i < 8; i++)
		{
			alt_configs[i] = 0;
		}

		for (i = 0; i < rows.length; i++)
		{
			rev_i = rows.length - i - 1;

			for (j = 0; j < rows[i].length; j++)
			{
				rev_j = rows[i].length - 1 - j;

				if ((config & 1L << rows[i][j]) == 0) continue;

				alt_configs[1] |= 1L << rows[i][rev_j];
				alt_configs[2] |= 1L << rows[rev_i][j];
				alt_configs[3] |= 1L << rows[rev_i][rev_j];
				alt_configs[4] |= 1L << cols[i][j];
				alt_configs[5] |= 1L << cols[i][rev_j];
				alt_configs[6] |= 1L << cols[rev_i][j];
				alt_configs[7] |= 1L << cols[rev_i][rev_j];
			}
		}

		return max (alt_configs);
	}

	public int compareTo (Object o)
	{
		long h1, h2;

		h1 = hash ();
		h2 = ((MyHiQ) o).hash ();

		if (h1 == h2) return 0;
		if (h1 < h2) return -1;
		return 1;
	}

	public boolean equals (Object o)
	{
		return this.compareTo (o) == 0;
	}
}
