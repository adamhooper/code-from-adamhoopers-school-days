import java.awt.*;
import javax.swing.*;
                                                                                
/**
 * A little button that gives the ability to create Wires.
 */
class WireCreator extends JComponent
{
    public int state = 0;
                                                                                
    // {{{ constructor
    public WireCreator()
    {
        this.setSize(130, 20);
        this.setForeground(Color.black);
        this.setBackground(Color.white);
    }
                                                                                
    // }}}
    // {{{ paintComponent()
    public void paintComponent(Graphics g)
    {
        super.paintComponent(g);
                                                                                
        String txt = "Add new Wire";
        if (state == 1)
            txt = "Choose first Node";
        if (state == 2)
            txt = "Choose second Node";
                                                                                
        g.drawRoundRect(getX(), getY(), getWidth(), getHeight(), 6, 6);
        g.drawString(txt, getX() + 5, getY() - 5 + getHeight());
    }
                                                                                
    // }}}
}
