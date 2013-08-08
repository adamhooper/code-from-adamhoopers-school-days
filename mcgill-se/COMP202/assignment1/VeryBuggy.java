// Name: Adam Hooper
// ID number: 260055737
// Course number: COMP202
// Section number: 002
// Assignment number: 1
// Where you developed your program: at home
//
// Fixed errors are displayed by comments prefixed by 'BROKEN'

public class VeryBuggy {
  public static void main(String[] args)
   {   // declare variables
       double priceHotelRoom, conferenceFees, planeTicket, total;
       //BROKEN: int numberDays; age;
       int numberDays, age;
       String speaker;
 
       // introduce program
       //BROKEN: System.out.println ("This program computes the total
       System.out.println ("This program computes the total "
                       + " expenses for a speaker at a conference. ");
 
       /* initialize variables */
       speaker = "Harry Potter";
	   //BROKEN: (pass 3) age = 7.0;
	   age = 7;
       numberDays = 4;
       //BROKEN: priceHotelRoom = 9o.00;
       priceHotelRoom = 90.00;
       //BROKEN: (pass 2) conferenceFee = 100.00
	   conferenceFees = 100.00;
       //BROKEN: planeTicket == 880.50;
       planeTicket = 880.50;
 
       // calculate total expenses
       //BROKEN: total = (numberDays + priceHotelRoom) + conferenceFees *
       total = (numberDays * priceHotelRoom) + conferenceFees +
               planeTicket;
 
        /* print output */
        //BROKEN: System.out.printn ( " Name: " + spaeker);
        System.out.println ( " Name: " + speaker);
        System.out.println ( " Age: " + age);
        System.out.println ( " Total spendings : " + total);
 
    } // main
} // class Buggy
