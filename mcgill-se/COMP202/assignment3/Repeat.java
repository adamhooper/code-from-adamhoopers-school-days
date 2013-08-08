//Name: Adam Hooper
//McGIll ID: 260055737
//Course: COMP202
//Section: 002
//Assignment: 3
//Developed at: home

import cs1.Keyboard;

public class Repeat
{
    static String repeatString(String s, int n)
    {
        String ret = "";

        if (n < 1)
            n = 2;

        while (n-- > 0)
            ret += s;

        return ret;
    }

    static String repeatString(String s)
    {
        return repeatString(s, 2);
    }

    public static void main(String[] argv)
    {
        System.out.print("Enter a word: ");
        String s = Keyboard.readString();

        System.out.print("Enter a number: ");
        int numTimes = Keyboard.readInt();

        System.out.println("Your word repeated twice: " +
                           repeatString(s));

        System.out.println("Your word repeated " + numTimes + " times: " +
                           repeatString(s, numTimes));
    }
}
