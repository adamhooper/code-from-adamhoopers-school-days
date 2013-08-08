import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.LinkedList;
                                                                                
/**
 * Represents the centre of a Gate.
 *
 * These hubs take time to fire(). Each time they're fire()d, a timer is created * to delay the fire()-ing until they really should.
 */
public abstract class GateHub extends Node
{
    protected LinkedList outputs; // Outputs we'll be giving (fifo)
                                                                                
    ActionListener fl;
                                                                                
    private final int TICK_INTERVAL = 1000;
                                                                                
    // {{{ constructor
    /**
     * Creates a new GateHub.
     */
    protected GateHub()
    {
        outputs = new LinkedList();
                                                                                
        //TODO: Maybe there's one chance in a million we'd have more Timers
        //      than LinkedList elements? If so, it'd crash here.
        fl = new ActionListener() {
            public void actionPerformed(ActionEvent ev) {
                Boolean b;
                b = (Boolean) outputs.removeFirst();
                signal = b.booleanValue();
                setOutputSignals();
            }
        };
    }
 
    // }}}
    // {{{ fire()
    /**
     * Fires the gate.
     *
     * Actually, it will store what the value <em>should</em> be and it'll set
     * up a timer to switch the value after the corresponding amount of time.
     */
    public void fire()
    {
        Timer timer;
        boolean value;
 
        value = this.fireInternal();
        outputs.add(new Boolean(value));
 
        timer = new Timer(this.getFiringDelay() * TICK_INTERVAL, fl);
        timer.setRepeats(false);
        timer.start();
    }
 
    // }}}
    // {{{ fireInternal()
    /**
     * Returns the value of the output based on the current inputs.
     *
     * This is where the logic of the gate goes (and/or/not).
     *
     * @return The value the gate should fire, based on inputs.
     */
    protected abstract boolean fireInternal();
 
    // }}}
    // {{{ getFiringDelay()
    /**
     * Returns the time delay this gate takes to fire, in &quot;ticks&quot;.
     */
    protected abstract int getFiringDelay();
 
    // }}}
}
