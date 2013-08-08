import java.awt.*;
import javax.swing.*;
                                                                                
/**
 * Represents an inverter in 2D space.
 */
public class Inverter extends Gate
{
    public Node input;
    public Node output;
                                                                                
    private InverterHub hub;
                                                                                
    private Image pic;
                                                                                
    // {{{ constructor
    /**
     * Builds a new inverter.
     *
     * <p>The gate looks like this:</p>
     * <pre>
     * ------*(input)--------------*(hub)-----------------*(output)
     * </pre>
     * <p>Note that input and output are public <code>Node</code>s.
     * They are built along with the <code>AndGate</code>.
     */
    public Inverter()
    {
        this.setSize(30, 20);
        this.setForeground(Color.green);
                                                                                
        // Build Nodes
        this.hub = new InverterHub();
        this.input = new Node();
        this.output = new Node();
                                                                                
        // Build Wires to connect the Nodes internally
        new Wire(input, hub);
        new Wire(hub, output);
                                                                                
        this.pic = getImage("inverter.png");
                                                                                
        setLocation(0, 0);
    }
                                                                                
    // }}}
    // {{{ setLocation()
    public void setLocation(int newX, int newY)
    {
        super.setLocation(newX, newY);
                                                                                
        int x = getX(), y = getY(), w = getWidth(), h = getHeight();
                                                                                
        hub.setLocation(x + w*1/3, y + h*1/4);
        input.setLocation(x, y + h*1/4);
        output.setLocation(x + w*3/4, y + h*1/4);
    }
                                                                                
    // }}}
    // {{{ paintComponent()
    /**
     * Draws the OR gate.
     *
     * @param g Circuit diagram Graphics.
     */
    public void paintComponent(Graphics g)
    {
        g.drawImage(this.pic, getX(), getY(), getWidth(), getHeight(), null);
                                                                                
        super.paintComponent(g);
    }
                                                                                
    // }}}
}
