// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP-202
// Section: 002
// Assignment: 4
// Developed at: home
//
// FullAdder: Takes three signals, outputs their sum

public class FullAdder
{
	private Wire in1, in2, carry_in;
	private Wire out, carry_out;

	private Wire h1_h2;
	private Wire h2_or;
	private Wire h1_or;

	private HalfAdder h1, h2;
	private OrGate or;

	// {{{ constructor
	/**
	 * Creates a full-adder circuit from three inputs and two outputs.
	 *
	 * @param in1 First input
	 * @param in2 Second input
	 * @param carry_in Third input
	 * @param out First output
	 * @param carry_out Second output
	 */
	public FullAdder(Wire in1, Wire in2, Wire carry_in, Wire out,
					 Wire carry_out)
	{
		this.in1 = in1;
		this.in2 = in2;
		this.carry_in = carry_in;
		this.out = out;
		this.carry_out = carry_out;

		this.h1_h2 = new Wire();
		this.h2_or = new Wire();
		this.h1_or = new Wire();

		this.h1 = new HalfAdder(in1, h1_h2, out, h2_or);
		this.h2 = new HalfAdder(in2, carry_in, h1_h2, h1_or);
		this.or = new OrGate(h2_or, h1_or, carry_out);

		this.h1_or.add_gate(or);

		// Corrected: this line wasn't in the original
		this.h2_or.add_gate(or);
	}

	// }}}
}
