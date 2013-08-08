import java.awt.*;
import javax.swing.*;
                                                                                
class OrGateCreator extends GateCreator
{
    public String getText() { return "Create new Or Gate"; }
    public Gate createGate() { return new OrGate(); }
}
