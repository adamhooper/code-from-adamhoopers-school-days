import java.util.ListIterator;
                                                                                
/**
 * Represents the hub of the OrGate.
 */
public class OrGateHub extends Node
{
    // {{{ fireInternal()
    /**
     * Returns output to OR of inputs.
     */
    public boolean fireInternal()
    {
        ListIterator li;
                                                                                
        li = this.inputs.listIterator(0);
        while (li.hasNext())
            if (((Wire) li.next()).getSignal())
                return true;
                                                                                
        return false;
    }
                                                                                
    // }}}
    protected int getFiringDelay() { return 5; }
}
