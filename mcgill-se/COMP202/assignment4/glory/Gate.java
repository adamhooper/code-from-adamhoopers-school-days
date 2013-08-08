import java.awt.*;
import javax.swing.*;
                                                                                
/**
 * Abstract class for reused Gate methods.
 */
public abstract class Gate extends JComponent
{
    // {{{ getImage()
    /**
     * Returns a new <code>Image</code> from the given (relative) path.
     *
     * @param path For example, "andgate.png"
     * @return     An <code>Image</code> from that path.
     */
    public Image getImage(String path)
    {
        ClassLoader l = getClass().getClassLoader();
        Toolkit t = Toolkit.getDefaultToolkit();
        return t.createImage(l.getResource(path));
    }

    // }}}
}
