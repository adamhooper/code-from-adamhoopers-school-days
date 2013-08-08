// Name: Adam Hooper
// ID number: 260055737
// Course number: COMP202
// Section number: 002
// Assignment number: 2
// Where you developed your program: at home
//
// Order.java: ask for 4 integers, print them in ascending order

import cs1.Keyboard;

public class Order
{
	public static void main (String[] _x)
	{
		int i1, i2, i3, i4;
		int x; // for swapping

		System.out.print("Enter the first number: ");
		i1 = Keyboard.readInt();
		System.out.print("Enter the second number: ");
		i2 = Keyboard.readInt();
		System.out.print("Enter the third number: ");
		i3 = Keyboard.readInt();
		System.out.print("Enter the fourth number: ");
		i4 = Keyboard.readInt();

		// Variable swapping shortened to one per swap

		// Order 1st/2nd, 3rd/4th
		if (i1 > i2) { x = i1; i1 = i2; i2 = x; }
		if (i3 > i4) { x = i3; i3 = i4; i4 = x; }

		// Order 1st/3rd, 2nd/4th
		if (i1 > i3) { x = i1; i1 = i3; i3 = x; }
		if (i2 > i4) { x = i2; i2 = i4; i4 = x; }

		// And 2nd/3rd
		if (i2 > i3) { x = i2; i2 = i3; i3 = x; }

		System.out.println("The ordered list is: "
							+ i1 + ", " + i2 + ", " + i3 + ", " + i4);
	}
}
