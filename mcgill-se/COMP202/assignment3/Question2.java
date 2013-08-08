//Name: Adam Hooper
//McGIll ID: 260055737
//Course: COMP202
//Section: 002
//Assignment: 3
//Developed at: home
//
//Question2: demonstrates ArithmeticTable

public class Question2
{
    public static void main(String[] argv) {
        ArithmeticTable multiplier = new ArithmeticTable(1, 20, '*');

        System.out.println("Multiplication Table From " + multiplier.getMin() +
                           " to " + multiplier.getMax() + ":");

        multiplier.printTable();
    }
}
