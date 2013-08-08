//Name: Adam Hooper
//McGIll ID: 260055737
//Course: COMP202
//Section: 002
//Assignment: 3
//Developed at: home
//
//Question3: Tests UnaryMath and UnaryNumber operations

public class Question3
{
    public static void main(String[] argv) {
       UnaryNumber u1 = new UnaryNumber("III");     // set u1 to 3 in unary
       UnaryNumber u2 = UnaryMath.convertToUnary(4); // set u2 to 4 in unary

       u1.multiply(u2);   // sets u1 to 12
       u1.add(u2);        // sets u1 to 16
       u1.divide(UnaryMath.convertToUnary(2));  // divides u1 by 2 (setting it to 8)

       UnaryNumber u3 = UnaryMath.abs(u2, u1);       // sets u3 to 4 in unary
       u3.subtract(UnaryMath.convertToUnary(2));     // sets u3 to 2 in unary

       UnaryNumber u4 = UnaryMath.power(u2, u3);   // sets u2 to 4^2 = 16 in unary

       System.out.println("u1 = " + u1.getValue());
       System.out.println("u2 = " + u2.getValue());
       System.out.println("u3 in decimal = " + UnaryMath.convertToDecimal(u3));
       System.out.println("u4 in decimal = " + UnaryMath.convertToDecimal(u4));
    }
}
