import java.util.*;

public class GateList
{
    Wire mywire;
    LinkedList the_list;
    public GateList(Wire w)
    {
        mywire = w;
        the_list = new LinkedList();
    }
    public void add_gate(Inverter g)
    {
        the_list.add(g);
    }
    public void add_gate(AndGate g)
    {
        the_list.add(g);
    }
    public void add_gate(OrGate g)
    {
        the_list.add(g);
    }
    public void propagate_signal()
    {
        ListIterator iterator = the_list.listIterator(0);
        while (iterator.hasNext()) {
            Object o = iterator.next();
            if (o instanceof Inverter)     ((Inverter)o).fire();
            else if (o instanceof AndGate) ((AndGate)o).fire();
            else if (o instanceof OrGate)  ((OrGate)o).fire();
        }
    }
}
