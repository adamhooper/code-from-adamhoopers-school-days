import java.util.Comparator;

public class TestHeapsort
{
	public static void main (String[] a)
	{
		if (a.length == 0)
		{
			System.err.println ("usage: java TestHeapsort "
					    + "[string1] [string1] ...");
			return;
		}

		Sorter.sort (a, String.CASE_INSENSITIVE_ORDER);

		for (int i = 0; i < a.length; i++)
		{
			System.out.println (a[i]);
		}
	}
}
