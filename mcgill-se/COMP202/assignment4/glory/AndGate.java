import java.awt.*;
import javax.swing.*;
                                                                                
/**
 * Represents an AND gate in 2D space.
 */
public class AndGate extends Gate
{
    public Node input1, input2;
    public Node output;
                                                                                
    private Image pic;
                                                                                
    private AndGateHub hub;
                                                                                
    // {{{ constructor
    /**
     * Builds a new AND gate.
     *
     * <p>The gate looks like this:</p>
     * <pre>
     * ------*(input1)\
     *                 \ (i1_hub)
     *                  \         (hub_output)
     *                   *(hub)-----------------*(output)
     *                  /
     *                 / (i2_hub)
     * ------*(input2)/
     * </pre>
     * <p>Note that input1, input2 and output are public <code>Node</code>s.
     * They are built along with the <code>AndGate</code>.
     */
    public AndGate()
    {
        this.setSize(40, 20);
        this.setForeground(Color.green);
                                                                                
        // Build Nodes
        this.hub = new AndGateHub();
                                                                                
        this.input1 = new Node();
        this.input2 = new Node();
        this.output = new Node();
                                                                                
        // Build Wires to connect the Nodes internally
        new Wire(input1, hub);
        new Wire(input2, hub);
        new Wire(hub, output);
                                                                                
        this.pic = getImage("andgate.png");
                                                                                
        setLocation(0, 0);
    }
                                                                                
    // }}}
    // {{{ setLocation()
    public void setLocation(int newX, int newY)
    {
        super.setLocation(newX, newY);
                                                                                
        int x = getX(), y = getY(), w = getWidth(), h = getHeight();
                                                                                
        hub.setLocation(x + w/2, y + h*1/4);
        input1.setLocation(x, y);
        input2.setLocation(x, y + h/2);
        output.setLocation(x + w*3/4, y + h*1/4);
    }
                                                                                
    // }}}
    // {{{ paintComponent()
    /**
     * Draws the AND gate.
     *
ram g Circuit diagram Graphics.
     */
    public void paintComponent(Graphics g)
    {
        g.drawImage(this.pic, getX(), getY(), getWidth(), getHeight(), null);
 
        super.paintComponent(g);
    }
 
    // }}}
}
