// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP-202
// Section: 002
// Assignment: 4
// Developed at: home
//
// OrGate: OR Gate

public class OrGate
{
	private Wire input1;
	private Wire input2;
	private Wire output;

	// {{{ constructor
	/**
	 * Creates a new OR Gate.
	 *
	 * @param input1 Top input wire
	 * @param input2 Second input wire
	 * @param output Output wire
	 */
	public OrGate(Wire input1, Wire input2, Wire output)
	{
		this.input1 = input1;
		this.input2 = input2;
		this.output = output;
	}

	// }}}
	// {{{ fire()
	/**
	 * Executes the gate's function (changes output signal).
	 */
	public void fire()
	{
		int val1 = input1.get_signal();
		int val2 = input2.get_signal();
		int out = 0;

		if (val1 == 1 || val2 == 1)
			out = 1;

		output.set_signal(out);
	}

	// }}}
}
