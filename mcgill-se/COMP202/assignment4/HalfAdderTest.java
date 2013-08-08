public class HalfAdderTest
{
	public static void main(String[] args)
	{
		Wire in1 = new Wire(), in2 = new Wire();
		Wire out1 = new Wire(), out2 = new Wire();

		HalfAdder adder = new HalfAdder(in1, in2, out1, out2);

		in1.set_signal(0);
		in2.set_signal(1);

		adder.fire();

		System.out.println("out1: " + out1.get_signal() + "; " +
						   "out2: " + out2.get_signal());
	}
}
