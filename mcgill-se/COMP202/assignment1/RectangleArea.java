// Name: Adam Hooper
// ID number: 260055737
// Course number: COMP202
// Section number: 002
// Assignment number: 1
// Where you developed your program: at home
//
// ---------------------------------------------------
//  RectangleArea.java (application)
//
//  Author:      Mr. Bill
//  Entered by:  Adam Hooper 260055737
//  Classes:     RectangleArea
//  Date:        May 1st, 2001
//  Description: Computing and printing the area of a rectangle.
//  Known Bugs:  None
// ---------------------------------------------------
 
public class RectangleArea
{
   public static void main(String[] args)
    {
      int width, length ;  // width and length of a rectangle
      int area;            // area of rectangle
 
      // initialize width and length
      width = 5;
      length = 12;
 
      // compute the area
      area = width * length;
 
      // print the result, and a goodbye message
     System.out.println (" The area of a rectangle of width: " +
               width +
                         " and length: " + length + " is >   " +
              area );
                         
     System.out.println ("Good Bye!");
   } // main
} // class CircleArea
