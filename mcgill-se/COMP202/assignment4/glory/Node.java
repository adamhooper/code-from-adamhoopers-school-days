import java.awt.*;
import javax.swing.*;
                                                                                
import java.util.LinkedList;
import java.util.ListIterator;
                                                                                
/**
 * Represents a point in 2D space where we can attach Wires.
 *
 * Nodes are the most important part of this applet: only Nodes can be
 * manipulated via the user-interface. Nodes allow the user to add an arbitrary
 * number if Wires of input and output. Nodes do not take time to fire().
 *
 * Most of the time the user will not realize what Nodes are. They are the
 * start and end of a Wire. A circuit must start with at least one Node, or
 * there would be nowhere to place Wires.
 *
 * Gates include Nodes so the user can attach Wires to them. An AndGate, for
 * example, will have two input Wires and an output Wire. These are represented
 * by two Nodes on the left and one on the right. Internally to the Gate, each
 * Node inside a Gate has an invisible Wire attaching it.
 *
 * When fired, a Node will send a signal to all its output wires if any one of
 * its input wires has a signal. Therefore, it's at the centre of the OrGate
 * as well.
 *
 * A Node is represented graphically by a circle.
 */
public class Node extends Gate
{
    protected boolean signal = false;
                                                                                
    LinkedList inputs;
    LinkedList outputs;
                                                                                
    // {{{ constructor
    /**
     * Creates a new Node with the given co-ordinates.
     *
     * @param x X-coordinate
     * @param y Y-coordinate
     */
    public Node()
    {
        inputs = new LinkedList();
        outputs = new LinkedList();
 
        this.setSize(10, 10);
        this.setForeground(Color.black);
    }
 
    // }}}
    // {{{ addInput()
    /**
     * Sets the passed <code>Wire</code> to be an input to this
     * <code>Node</code>.
     *
     * @param input New input <code>Wire</code>.
     */
    public void addInput(Wire input)
    {
        this.inputs.add(input);
    }
 
    // }}}
    // {{{ addOutput()
    /**
     * Sets the passed <code>Wire</code> to be an output from this
     * <code>Node</code>.
     *
     * @param output New output <code>Wire</code>.
     */
    public void addOutput(Wire output)
    {
        this.outputs.add(output);
    }
 
    // }}}
    // {{{ paintComponent()
    /**
     * Paints this node onto our circuit diagram.
     *
     * @param g Circuit diagram <code>Graphics</code>.
     */
    public void paintComponent(Graphics g)
    {
        if (signal)
            g.setColor(Color.blue);
 
        g.fillOval(getX(), getY(), getWidth(), getHeight());
 
        super.paintComponent(g);
    }
 
    // }}}
    // {{{ fire()
    /**
     * Changes signal of output <code>Wire</code>s depending on the input
     * <code>Wire</code>s' signals.
     *
     * If any input <code>Wire</code> has a signal, <em>all</em> the output
     * <code>Wire</code>s will be given a signal.
     */
    public void fire()
    {
        this.getSignal();
 
        this.setOutputSignals();
    }
 
    // }}}
    // {{{ toggle()
    /**
     * Sets all output Wires to the opposite signal of what they now have.
     */
    public void toggle()
    {
        this.signal = !this.signal;
 
        this.setOutputSignals();
    }
 
    // }}}
 
    // {{{ getSignal()
    /**
     * Sets signal to true if any of the inputs has a signal.
     */
    private void getSignal()
    {
        ListIterator li;
 
        li = this.inputs.listIterator(0);
        while (li.hasNext())
            if (((Wire) li.next()).getSignal()) {
                this.signal = true;
                return;
            }
 
        this.signal = false;
    }
 
    // }}}
    // {{{ setOutputSignals()
    /**
     * Sets all output <code>Wire</code>s to the signals they should have.
     */
    protected void setOutputSignals()
    {
        ListIterator li;
                                                                                
        li = this.outputs.listIterator(0);
        while (li.hasNext())
            ((Wire) li.next()).setSignal(this.signal);
 
        repaint();
    }
 
    // }}}
}
