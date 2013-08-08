// Name: Adam Hooper
// ID number: 260055737
// Course number: COMP202
// Section number: 002
// Assignment number: 1
// Where you developed your program: at home
//
// GloryApplet: Displays a happy face and some personal information

import java.applet.Applet;
import java.awt.*;

public class GloryApplet extends Applet
{
	public void paint (Graphics page)
	{
		page.drawOval (10, 10, 50, 50);
		page.drawOval (25, 25,  5,  5);
		page.drawOval (45, 25,  5,  5);
		page.drawArc  (20, 40, 30, 10, 180, 180);

		page.drawString ("Name: Adam HOOPER", 80, 30);
		page.drawString ("McGill Id: 260055737", 80, 45);
		page.drawString ("Degree Program: B. Eng.", 80, 60);
		page.drawString ("Major: Software Engineering", 80, 75);
		page.drawString ("Have a PC at home? Yes", 80, 90);
		page.drawString ("Connected to the Internet? Yes", 80, 105);
		page.drawString ("Installed Java and JCreator? JDK: Yes", 80, 120);
		page.drawString ("Message for TA: Hello!", 80, 135);
	}
}
