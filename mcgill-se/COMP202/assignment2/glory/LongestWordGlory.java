// Name: Adam Hooper
// ID number: 260055737
// Course number: COMP202
// Section number: 002
// Assignment number: 2
// Where you developed your program: at home
//
// LongestWordGlory.java: From an input string, output the longest word
//
// (temporarily) available at
// http://www.adamhooper.com:4243/school/COMP202/ass2/glory/index.html

import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;

public class LongestWordGlory extends Applet
							  implements ActionListener
{
	private TextField inputField;
	private Button bFindLongestWord, bClear, bQuit;
	private Label lInputField, lAnswer;

	// {{{ init()
	/**
	 * Lays out labels and text boxes and stuff
	 */
	public void init()
	{
		lInputField = new Label("Enter a sentence: ");
		add(lInputField);

		inputField = new TextField("", 30);
		add(inputField);

		bFindLongestWord = new Button("Find longest word");
		add(bFindLongestWord);

		bClear = new Button("Clear");
		add(bClear);

		lAnswer = new Label("Waiting for input...                    ");
		add(lAnswer);

		bFindLongestWord.addActionListener(this);
		bClear.addActionListener(this);
	}

	// }}}
	// {{{ actionPerformed()
	/**
	 * Processes action signals
	 */
	public void actionPerformed(ActionEvent e)
	{
		if (e.getSource() == bFindLongestWord)
			actionFind();
		if (e.getSource() == bClear)
			actionClear();
	}

	// }}}

	// {{{ findLongestWord()
	/**
	 * Returns the longest word in the text box
	 * @return String
	 */
	private String findLongestWord()
	{
		String input;

		String longestWord = "", curWord = "";
		char curLetter;

		int i;

		input = inputField.getText();

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

		return longestWord;
	}

	// }}}
	// {{{ actionClear()
	/**
	 * Clears the text box
	 */
	private void actionClear()
	{
		inputField.setText("");
		lAnswer.setText("Waiting for input...");
	}

	// }}}
	// {{{ actionFind()
	/**
	 * Determines the longest word and puts it into lAnswer
	 */
	private void actionFind()
	{
		lAnswer.setText("Longest word: " + findLongestWord());
	}

	// }}}
}

// vim: set sw=4 ts=4 noet:
// vim600: set foldmethod=marker:
