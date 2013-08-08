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
public class Colors extends Applet implements ActionListener
{
    Label label = new Label("Type in a color:");
    TextField input = new TextField(5);

    // {{{ init()
    /**
     * Sets up our components.
     */
    public void init()
    {
        label.setForeground(Color.black);
        this.add(label);

        this.add(input);
        input.addActionListener(this);
    }

    // }}}
    // {{{ paint()
    /**
     * Draws our circle.
     */
    public void paint(Graphics g)
    {
        super.paint(g);
        g.fillOval(50, 50, 100, 100);
    }

    // }}}
    // {{{ actionPerformed()
    /**
     * Responds to user actions.
     *
     * If somebody pressed Enter in the text field, change the circle's color.
     */
    public void actionPerformed(ActionEvent e)
    {
        Object o = e.getSource();

        if (o == input) {
            updateColor();
        }
    }

    // }}}
    // {{{ updateColor()
    /**
     * Changes the color of the circle.
     *
     * I used if-else because it's the simplest (though it may be the ugliest).
     */
    private void updateColor()
    {
        String t = input.getText();

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
