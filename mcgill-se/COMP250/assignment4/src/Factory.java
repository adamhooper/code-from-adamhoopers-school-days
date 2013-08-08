import java.util.Stack;

public class Factory
{
	private Stack bin;

	public Factory ()
	{
		bin = new Stack ();
	}

	public MyHiQ getBoard ()
	{
		if (bin.isEmpty ())
		{
			return new MyHiQ ();
		}
		else
		{
			return (MyHiQ) bin.pop ();
		}
	}

	public void recycle (MyHiQ board)
	{
		bin.push (board);
	}

	public MyHiQ clone (MyHiQ hiq)
	{
		MyHiQ ret = getBoard ();
		ret.config = hiq.config;
		return ret;
	}
}
