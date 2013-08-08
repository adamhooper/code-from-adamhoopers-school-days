import java.awt.*;
import javax.swing.*;
                                                                                
class InverterCreator extends GateCreator
{
    public String getText() { return "Create new Inverter"; }
    public Gate createGate() { return new Inverter(); }
}
