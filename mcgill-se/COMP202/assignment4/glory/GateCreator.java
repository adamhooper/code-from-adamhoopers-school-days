import java.awt.*;import javax.swing.*;
                                                                                
/**
 * Provides a box which will add a Gate to the Canvas when clicked.
 */
public abstract class GateCreator extends JComponent
{
    // {{{ constructor
    public GateCreator()
    {
        setForeground(Color.black);
        setBackground(Color.white);
        setSize(130, 20);
    }
    // }}}
    // {{{ getText()
    /**
     * Gives the text to put in the graphical representation.
     *
     * @return Text for this Gate Creator.
     */
    public abstract String getText();
                                                                                
    // }}}
    // {{{ createGate()
    /**
     * Creates the desired gate and returns it.
     *
     * @return The desired gate.
     */
    public abstract Gate createGate();
                                                                                
    // }}}
    // {{{ paintComponent()
    /**
     * Paints the little button.
     *
     * @param g Graphics object with which to paint.
     */
    public void paintComponent(Graphics g)
    {
        super.paintComponent(g);
                                                                                
        g.drawRoundRect(getX(), getY(), getWidth(), getHeight(), 6, 6);
        g.drawString(this.getText(), getX() + 5, getY() - 5 + getHeight());
    }
                                                                                
    // }}}
}
