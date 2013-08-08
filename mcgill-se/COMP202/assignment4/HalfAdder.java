// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP-202
// Section: 002
// Assignment: 4
// Developed at: home
//
// HalfAdder: Takes two signals, outputs XOR on top, AND on bottom

public class HalfAdder
{
	private Wire in1, in2, out1, out2;
	private Wire or_and2, inv_and2;

	private OrGate or;
	private AndGate and1, and2;
	private Inverter inv;

	// {{{ constructor
	/**
	 * Creates a half-adder circuit from two inputs and two outputs.
	 *
	 * @param in1 Top input wire
	 * @param in2 Bottom input wire
	 * @param out1 Top output wire (XOR'd)
	 * @param out2 Bottom output wire (AND'd)
	 */
	public HalfAdder(Wire in1, Wire in2, Wire out1, Wire out2)
	{
		this.in1 = in1;
		this.in2 = in2;
		this.out1 = out1;
		this.out2 = out2;

		this.or_and2 = new Wire();
		this.inv_and2 = new Wire();

		this.or = new OrGate(in1, in2, or_and2);
		this.and1 = new AndGate(in1, in2, out2);
		this.inv = new Inverter(out2, inv_and2);
		this.and2 = new AndGate(or_and2, inv_and2, out1);

		this.in1.add_gate(or);
		this.in1.add_gate(and1);

		this.in2.add_gate(or);
		this.in2.add_gate(and1);

		this.out2.add_gate(inv);

		this.or_and2.add_gate(and2);
		this.inv_and2.add_gate(and2);
	}

	// }}}
}
