// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 6
// Developed at: Home
//
// Colors: Draw a circle filled with the user-input file

import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;

/**
 * Asks the user for a color and fills a circle of that color.
 *
 * The rest of the comments in this code are grudgingly appended so no marks
 * will be lost. Feel free to ignore them: the code documents itself better
 * than comments ever could.
 */
public class Colors extends Applet implements ItemListener
{
    Label label = new Label("Choose a colour and shape: ");
    Choice color = new Choice();
    Choice shape = new Choice();

    // {{{ init()
    /**
     * Sets up our components.
     */
    public void init()
    {
        label.setForeground(Color.black);
        this.add(label);

        color.add("Red");
        color.add("Green");
        color.add("Blue");
        this.add(color);
        color.addItemListener(this);

        shape.add("Circle");
        shape.add("Square");
        shape.add("Triangle");
        this.add(shape);
        shape.addItemListener(this);

        updateColor();
    }

    // }}}
    // {{{ paint()
    /**
     * Draws our shape.
     */
    public void paint(Graphics g)
    {
        super.paint(g);

        String s = shape.getSelectedItem();

        if (s.compareToIgnoreCase("circle") == 0) {
            g.fillOval(50, 50, 100, 100);
        } else if (s.compareToIgnoreCase("square") == 0) {
            g.fillRect(50, 50, 100, 100);
        } else if (s.compareToIgnoreCase("triangle") == 0) {
            int x[] = { 50, 100, 50 };
            int y[] = { 50, 100, 100 };
            g.fillPolygon(x, y, 3);
        }
    }

    // }}}
    // {{{ itemStateChanged()
    /**
     * Responds to user actions.
     */
    public void itemStateChanged(ItemEvent e)
    {
        Object o = e.getSource();

        if (o == color) {
            updateColor();
        }
        else if (o == shape) {
            repaint();
        }
    }

    // }}}
    // {{{ updateColor()
    /**
     * Changes the color of the shape.
     *
     * I used if-else because it's the simplest (though it may be the ugliest).
     */
    private void updateColor()
    {
        String t = color.getSelectedItem();

        if (t.compareToIgnoreCase("red") == 0)
            setForeground(Color.red);
        else
            if (t.compareToIgnoreCase("green") == 0)
                setForeground(Color.green);
            else
                if (t.compareToIgnoreCase("blue") == 0)
                    setForeground(Color.blue);
                else
                    setForeground(Color.black);

        repaint();
    }

    // }}}
}
