import java.util.ListIterator;
                                                                                
/**
 * Represents the hub of the AndGate.
 */
public class AndGateHub extends GateHub
{
    // {{{ fireInternal()
    /**
     * Returns AND of inputs.
     */
    public boolean fireInternal()
    {
        ListIterator li;
                                                                                
        li = this.inputs.listIterator(0);
        while (li.hasNext())
            if (!((Wire) li.next()).getSignal())
                return false;
                                                                                
        return true;
    }
                                                                                
    // }}}
    protected int getFiringDelay() { return 3; }
}
