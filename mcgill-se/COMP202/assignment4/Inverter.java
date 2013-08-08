// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP-202
// Section: 002
// Assignment: 4
// Developed at: home
//
// Inverter: Inverter gate

public class Inverter
{
	private Wire input;
	private Wire output;

	// {{{ constructor
	/**
	 * Creates a new <code>Inverter</code>.
	 *
	 * @param input Wire leading in
	 * @param output Wire leaving
	 */
	public Inverter(Wire input, Wire output)
	{
		this.input = input;
		this.output = output;
	}

	// }}}
	// {{{ fire()
	/**
	 * Executes the gate's function (changes output signal).
	 */
	public void fire()
	{
		int val = input.get_signal();

		val++;
		val %= 2;

		output.set_signal(val);
	}

	// }}}
}
