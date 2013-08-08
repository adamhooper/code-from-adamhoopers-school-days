import java.util.Comparator;
import java.util.Arrays;
import java.util.List;

public class Sorter
{
	private int heapSize;
	private Object[] a;
	private Comparator c;

	public static void sort (Object[] a, Comparator c)
	{
		Sorter o = new Sorter (a, c);
		o.heapSort ();
	}

	public static void sort (List L, Comparator c)
	{
		Object[] a;

		a = L.toArray ();
		sort (a, c);

		L.clear ();
		L.addAll (Arrays.asList (a));
	}

	private Sorter (Object[] a, Comparator c)
	{
		this.a = a;
		this.c = c;
	}

	private void heapSort ()
	{
		buildHeap ();

		for (int i = heapSize - 1; i >= 1; i--)
		{
			exchange (0, i);
			heapSize--;
			heapify (0);
		}
	}

	private int parent (int i)
	{
		return (i + 1) / 2 - 1;
	}

	private int left (int i)
	{
		return 2 * (i + 1) - 1;
	}

	private int right (int i)
	{
		return 2 * (i + 1);
	}

	private void exchange (int i, int j)
	{
		Object t = a[i];
		a[i] = a[j];
		a[j] = t;
	}

	/*
	 * Iterates up heap (after inserting an element at i) to keep it a heap
	 */
	private void heapify (int i)
	{
		int l = left (i);
		int r = right (i);
		int largest;

		largest = i;

		if (l < heapSize && c.compare (a[l], a[largest]) > 0)
		{
			largest = l;
		}

		if (r < heapSize && c.compare (a[r], a[largest]) > 0)
		{
			largest = r;
		}

		if (largest != i)
		{
			exchange (i, largest);
			heapify (largest);
		}
	}

	/*
	 * Makes the array a into a heap
	 */
	private void buildHeap ()
	{
		heapSize = a.length;

		for (int i = heapSize / 2; i >= 0; i--)
		{
			heapify (i);
		}
	}
}
