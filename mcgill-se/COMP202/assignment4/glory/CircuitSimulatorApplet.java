import java.awt.*;
import javax.swing.*;
                                                                                
public class CircuitSimulatorApplet extends JApplet
{
    public CircuitCanvas canvas;
    public void init()
    {
        this.setForeground(Color.black);
        this.setBackground(Color.white);
                                                                                
        canvas = new CircuitCanvas();
        this.getContentPane().add(canvas);
    }
}
