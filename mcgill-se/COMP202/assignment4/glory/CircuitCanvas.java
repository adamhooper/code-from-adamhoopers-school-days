import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.Vector;
                                                                                
/**
 * A canvas for drawing a circuit.
 *
 * The CircuitCanvas is the heart of the GUI. It contains a bunch of circuit
 * elements (<code>Gate</code>s, <code>Node</code>s). These can be dragged
 * around.
 *
 * Note that <code>Wire</code>s are not added to this class: they are handled
 * by the <code>Node</code>s that output them.
 */
public class CircuitCanvas extends JPanel
{
    private Vector elems;
                                                                                
    // {{{ constructor
    /**
     * Creates a new CircuitCanvas.
     */
    public CircuitCanvas()
    {
        CircuitCanvasMouseListener ml = new CircuitCanvasMouseListener();
                                                                                
        this.setSize(600, 450);
                                                                                
        this.setBackground(Color.white);
        this.addMouseListener(ml);
        this.addMouseMotionListener(ml);
                                                                                
        elems = new Vector();
                                                                                
        buildCreators();
                                                                                
        /*
         * FIXME: EVIL HACK
         *
         * Now, doesn't this highlight the horrible structure quite well...:
         *
         * This Canvas does all the drawing. Which means that each JComponent
         * doesn't draw stuff. When the components change and need to redraw,
         * there are two options:
         *
         * 1. Give each component a reference to this canvas
         * 2. Don't repaint.
         *
         * Since I'm putting a timer here to repaint every few ms, obviously I
         * chose 2.
         *
         * It's lucky JPanel double-buffers.
         */
        ActionListener al = new ActionListener() {
            public void actionPerformed(ActionEvent ev) {
                repaint();
            }
        };
 
        Timer t = new Timer(100, al);
        t.start();
    }
 
    // }}}
    // {{{ buildCreators()
    /**
     * Adds all the Gate Creators to the canvas.
     */
    private void buildCreators()
    {
        WireCreator wc = new WireCreator();
        NodeCreator nc = new NodeCreator();
        InverterCreator ic = new InverterCreator();
        AndGateCreator ac = new AndGateCreator();
        OrGateCreator oc = new OrGateCreator();
 
        wc.setLocation(450, 300);
        nc.setLocation(450, 330);
        ic.setLocation(450, 360);
        ac.setLocation(450, 390);
        oc.setLocation(450, 420);
 
        add(wc);
        add(nc);
        add(ic);
        add(ac);
        add(oc);
    }
 
    // }}}
    // {{{ add()
    /**
     * Adds the circuit element to the canvas.
     *
     * @param e Element to add.
     */
    public void add(JComponent e)
    {
        elems.add(e);
        repaint();
    }
 
    // }}}
    // {{{ addGate()
    /**
     * Adds a Gate to the canvas.
     *
     * @param g Gate to add.
     */
    public void addGate(Gate g)
    {
        if (g instanceof AndGate) {
            AndGate a = (AndGate) g;
            this.add(a.input1);
            this.add(a.input2);
            this.add(a.output);
        }
 
        if (g instanceof OrGate) {
            OrGate o = (OrGate) g;
            this.add(o.input1);
            this.add(o.input2);
            this.add(o.output);
        }
 
        if (g instanceof Inverter) {
            Inverter i = (Inverter) g;
            this.add(i.input);
            this.add(i.output);
        }
 
        this.add(g);
    }
 
    // }}}
    // {{{ remove()
    /**
     * Removes the circuit element from the canvas.
     *
     * @param e Element to remove.
     */
    public void remove(JComponent e)
    {
        elems.remove(e);
        repaint();
    }
 
    // }}}
    // {{{ paint()
    /**
     * Paints all the elements on the canvas.
     *
     * @param g Graphics object with which to paint.
     */
    public void paint(Graphics g)
    {
        JComponent c;
 
        super.paint(g);
 
        for (int i = 0; i < elems.size(); i++) {
            c = (JComponent) elems.elementAt(i);
            c.paint(g);
        }
    }
 
    // }}}
 
    // {{{ CircuitCanvasMouseListener
    /**
     * A mouse listener for CircuitCanvas.
     *
     * The mouse listener will handle click-and-drag events, and will flip the
     * signal of a <code>Node</code> on clicks.
     */
    class CircuitCanvasMouseListener extends MouseInputAdapter
    {
        private JComponent selectedElement;
        private Point startPoint; // Offset from top-left of selectedElement
 
        private Node wireFromNode;
        private WireCreator creator = null;
 
        // {{{ mousePressed()
        public void mousePressed(MouseEvent e)
        {
            JComponent c;
            Rectangle r = null;
 
            // Latch onto the top circuit element we can move
            for (int i = elems.size() - 1; i >= 0; i--) {
                c = (JComponent) elems.elementAt(i);
 
                if (c instanceof Wire) continue;
 
                r = c.getBounds(r);
 
                if (r.contains(e.getPoint())) {
                    selectedElement = c;
                    startPoint = new Point(e.getX() - c.getX(),
                                           e.getY() - c.getY());
 
                    break;
                }
            }
        }
 
        // }}}
        // {{{ mouseDragged()
        public void mouseDragged(MouseEvent e)
        {
            if (selectedElement == null || startPoint == null)
                return;
 
            selectedElement.setLocation(e.getX() - (int) startPoint.getX(),
                                        e.getY() - (int) startPoint.getY());
 
            repaint();
        }
 
        // }}}
        // {{{ mouseReleased()
        public void mouseReleased(MouseEvent e)
        {
            selectedElement = null;
            startPoint = null;
        }
 
        // }}}
        // {{{ mouseClicked()
        public void mouseClicked(MouseEvent e)
        {
            JComponent c;
            Object o;
            Rectangle r = null;
 
            for (int i = elems.size() - 1; i >= 0; i--) {
                o = elems.elementAt(i);
 
                c = (JComponent) o;
                r = c.getBounds(r);
 
                if (!r.contains(e.getPoint()))
                    continue;
 
                if (o instanceof WireCreator) {
                    addWire((WireCreator) o);
                    repaint();
                    break;
                }
 
                if (o instanceof GateCreator) {
                    addGate(((GateCreator) o).createGate());
                    repaint();
                    break;
                }
 
                if (o instanceof Node) {
                    Node n = (Node) o;
 
                    if (this.creator == null) {
                        // We aren't adding a Wire: toggle the node.
                        n.toggle();
                        repaint();
                        break;
                    }
 
                    // Okay, we're in the process of adding a Wire.
                    if (this.wireFromNode != null
                            && this.wireFromNode != n) {
                        add(new Wire(this.wireFromNode, n));
                        wireFromNode.fire();
                        wireFromNode = null;
                        creator.state = 0;
                        creator = null;
                    } else {
                        this.wireFromNode = n;
                        creator.state = 2;
                    }
 
                    repaint();
                    break;
                }
            }
        }
 
        // }}}
        // {{{ addWire()
        /**
         * Starts phase 1 of adding a wire.
         *
         * If we're already adding a wire, cancel it.
         *
         * @param c Wire creator.
         */
        private void addWire(WireCreator c)
        {
            if (this.creator == null) {
                this.creator = c;
                c.state = 1;
            } else {
                c.state = 0;
                this.creator = null;
            }
        }
 
        // }}}
    }
 
    // }}}
}
