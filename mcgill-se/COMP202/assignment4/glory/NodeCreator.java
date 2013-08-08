import java.awt.*;
import javax.swing.*;
                                                                                
class NodeCreator extends GateCreator
{
    public String getText() { return "Create new Node"; }
    public Gate createGate() { return new Node(); }
}
