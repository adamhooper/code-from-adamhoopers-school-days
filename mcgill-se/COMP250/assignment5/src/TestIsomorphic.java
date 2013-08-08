import java.util.StringTokenizer;
import java.io.*;

public class TestIsomorphic
{
	public static void main (String[] args)
		throws IOException, FileNotFoundException
	{
		FileReader fr;
		BufferedReader br;
		String treeStr1, treeStr2;
		RootedTree t1, t2;
		int spaceIndex;
		Map map;

		if (args.length != 1)
		{
			System.err.println ("usage: "
					    + "java TestIsomorphic [file]");
			System.exit (1);
		}

		fr = new FileReader (args[0]);
		br = new BufferedReader (fr);

		treeStr1 = br.readLine (); // first tree as a string
		treeStr2 = br.readLine (); // second tree as a string

		if (treeStr2 == null) // trees separated by ' ', not '\n'
		{
			spaceIndex = treeStr1.indexOf (' ');

			if (spaceIndex == -1)
			{
				System.err.println ("Cannot parse file");
				System.exit (1);
			}

			treeStr2 = treeStr1.substring (spaceIndex + 1);
			treeStr1 = treeStr1.substring (0, spaceIndex - 1);
		}

		// Generate trees from strings
		t1 = new RootedTree (treeStr1);
		t2 = new RootedTree (treeStr2);

		// Test them
		System.out.println ("Tree 1: " + t1);
		System.out.println ("Tree 2: " + t2);

		map = RootedTree.isomorphic (t1, t2);

		if (map == null)
		{
			System.out.println ("The trees are not isomorphic.");
		}
		else
		{
			System.out.println ("An isomorphism exists:");
			System.out.println (map);
		}
	}
}
