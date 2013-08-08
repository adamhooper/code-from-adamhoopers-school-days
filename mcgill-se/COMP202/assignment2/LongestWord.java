// Name: Adam Hooper
// ID number: 260055737
// Course number: COMP202
// Section number: 002
// Assignment number: 2
// Where you developed your program: at home
//
// LongestWord.java: From an input string, output the longest word

import cs1.Keyboard;

public class LongestWord
{
	public static void main (String[] _x)
	{
		String input;

		String longestWord = "", curWord = "";
		char curLetter;

		int i;

		System.out.print("Type a phrase: ");
		input = Keyboard.readString();

		for (i = 0; i < input.length(); i++) {
			curLetter = input.charAt(i);

			if (curLetter != ' ') {
				// If it's a letter, add it to the current word
				curWord += curLetter;
			} else {
				// We're in between words, so start up a new one

				if (curWord.length() > longestWord.length())
					longestWord = curWord;

				curWord = "";
			}
		}

		// The last word may be the longest...
		if (curWord.length() > longestWord.length())
			longestWord = curWord;

		System.out.println("Longest word is: " + longestWord);
	}
}
