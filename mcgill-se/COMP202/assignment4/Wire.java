// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP-202
// Section: 002
// Assignment: 4
// Developed at: home
//
// Wire: Has a signal, and gets connected to stuff.

public class Wire
{
	private int signal = 0;
	private GateList gl;

	// {{{ Wire()
	/**
	 * Creates a new Wire.
	 */
	public Wire()
	{
		this.gl = new GateList(this);
	}

	// }}}
	// {{{ add_gate()
	/**
	 * Adds a gate for this wire to fire when its signal changes.
	 *
	 * @param gate Gate to fire when signal changes.
	 */
	public void add_gate(Inverter gate)
	{
		this.gl.add_gate(gate);
	}

	/**
	 * Adds a gate for this wire to fire when its signal changes.
	 *
	 * @param gate Gate to fire when signal changes.
	 */
	public void add_gate(AndGate gate)
	{
		this.gl.add_gate(gate);
	}

	/**
	 * Adds a gate for this wire to fire when its signal changes.
	 *
	 * @param gate Gate to fire when signal changes.
	 */
	public void add_gate(OrGate gate)
	{
		this.gl.add_gate(gate);
	}

	// }}}
	// {{{ set_signal()
	/**
	 * Sets the signal of the wire.
	 *
	 * @param value Signal value
	 */
	public void set_signal(int value)
	{
		if (value != 0 && value != 1) {
			System.out.println("Invalid value passed to set_signal(): " +
							   value);
		}

		this.signal = value;

		this.gl.propagate_signal();
	}

	// }}}
	// {{{ get_signal()
	/**
	 * Returns the value of this wire's signal.
	 *
	 * @return Signal value
	 */
	public int get_signal()
	{
		return this.signal;
	}

	// }}}
}
