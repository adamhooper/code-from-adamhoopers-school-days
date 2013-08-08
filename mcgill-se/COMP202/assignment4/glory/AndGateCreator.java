import java.awt.*;
import javax.swing.*;
                                                                                
class AndGateCreator extends GateCreator
{
    public String getText() { return "Create new And Gate"; }
    public Gate createGate() { return new AndGate(); }
}
