// Name: Adam Hooper
// ID number: 260055737
// Course number: COMP202
// Section number: 002
// Assignment number: 2
// Where you developed your program: at home
//
// Distance.java: 

import cs1.Keyboard;
import java.lang.Math;

public class Distance
{
	public static void main (String[] _x)
	{
		float x1, y1;
		float x2, y2;

		double xDist, yDist;

		double euclidDist, manhattanDist;

		System.out.println("Enter the first point:");
		System.out.print(" Enter the first coordinate (x1): ");
		x1 = Keyboard.readFloat();
		System.out.print(" Enter the second coordinate (y1): ");
		y1 = Keyboard.readFloat();

		System.out.println("Enter the second point:");
		System.out.print(" Enter the first coordinate (x2): ");
		x2 = Keyboard.readFloat();
		System.out.print(" Enter the second coordinate (y2): ");
		y2 = Keyboard.readFloat();

		xDist = Math.abs(x1 - x2);
		yDist = Math.abs(y1 - y2);

		euclidDist = Math.sqrt(Math.pow(xDist, 2) + Math.pow(yDist, 2));
		manhattanDist = xDist + yDist;

		System.out.println("The euclidian distance between "
							+ "(" + x1 + "," + y1 + ") and "
							+ "(" + x2 + "," + y2 + ") is "
							+ euclidDist);
		System.out.println("The Manhattan distance between "
							+ "(" + x1 + "," + y1 + ") and "
							+ "(" + x2 + "," + y2 + ") is "
							+ manhattanDist);
	}
}
