import java.util.ListIterator;
                                                                                
/**
 * Represents the hub of the Inverter
 */
public class InverterHub extends GateHub
{
    // {{{ fireInternal()
    /**
     * Returns NOT of input.
     */
    public boolean fireInternal()
    {
        return !(((Wire) inputs.getFirst()).getSignal());
    }
                                                                                
    // }}}
    protected int getFiringDelay() { return 2; }
}
