import java.awt.*;
import javax.swing.*;
                                                                                
/**
 * Represents a wire from one <code>Node</code> to another.
 *
 * A <code>Wire</code> will connect two <code>Node</code>s. Its signal will
 * change when <code>setSignal()</code> is called on it, which in turn will
 * fire() the <code>Node</code> at its output.
 */
public class Wire extends JComponent
{
    private boolean signal = false;
    private Node input, output;
                                                                                
    // {{{ constructor
    /**
     * Creates a new <code>Wire</code>.
     *
     * @param input The <code>Node</code> which gives the signal.
     * @param output The <code>Node</code> which receives the signal.
     */
    public Wire(Node input, Node output)
    {
        output.addInput(this);
        input.addOutput(this);
 
        this.input = input;
        this.output = output;
 
        // FIXME: Has to be whole canvas size to redraw...
        this.setSize(600, 450);
        this.setForeground(Color.black);
    }
 
    // }}}
    // {{{ getSignal()
    /**
     * Returns the signal (either <code>true</code> or <code>false</code>).
     *
     * @return signal The signal of this <code>Wire</code>.
     */
    public boolean getSignal()
    {
        return this.signal;
    }
 
    // }}}
    // {{{ setSignal()
    /**
     * Sets this <code>Wire</code>'s signal.
     *
     * @param signal The new signal for this <code>Wire</code>.
     */
    public void setSignal(boolean signal)
    {
        this.signal = signal;
        this.output.fire();
        repaint();
    }
 
    // }}}
    // {{{ paintComponent()
    /**
     * Draws a line.
     *
     * @param g Our drawing area.
     */
    public void paintComponent(Graphics g)
    {
        if (signal)
            g.setColor(Color.blue);
 
        g.drawLine(input.getX() + input.getWidth() / 2,
                   input.getY() + input.getHeight() / 2,
                   output.getX() + output.getWidth() / 2,
                   output.getY() + output.getHeight() / 2);
 
        super.paintComponent(g);
    }
 
    // }}}
}
