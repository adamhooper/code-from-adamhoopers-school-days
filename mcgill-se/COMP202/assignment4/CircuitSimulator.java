import cs1.Keyboard;

public class CircuitSimulator
{
    public static void main(String[] args)
    {
        Wire a = new Wire(), b = new Wire();
        Wire sum = new Wire();
        Wire carry_in = new Wire(), carry_out = new Wire();

        FullAdder adder = new FullAdder(a, b, carry_in, sum, carry_out);

        System.out.print("Enter A: ");
        int a_val = Keyboard.readInt();
        System.out.print("Enter B: ");
        int b_val = Keyboard.readInt();
        System.out.print("Enter Carry In: ");
        int ci_val = Keyboard.readInt();

        a.set_signal(a_val);
        b.set_signal(b_val);
        carry_in.set_signal(ci_val);

        int sum_val = sum.get_signal();
        int co_val = carry_out.get_signal();

        System.out.println("Sum = "+sum_val);
        System.out.println("Carry Out = "+co_val);
    }
}
