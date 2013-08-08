// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 5
// Developed at: Home
//
// ImportantEmail: You guessed it! An important email.

public class ImportantEmail extends Email
{
    // {{{ constructor
    /**
     * Create a new ImportantEmail object.
     */
    public ImportantEmail() { super(); }

    public ImportantEmail(String sender, String subject, int ageInDays)
    {
        super(sender, subject, ageInDays);
    }

    // }}}
    // {{{ getPriority()
    public int getPriority()
    {
        return 5 + 5 * getAge();
    }

    // }}}
    // {{{ toString()
    /**
     * Puts 'IMPORTANT' before the default <code>Email</code> string
     * representation.
     *
     * @return String representation of this Email.
     */
    public String toString()
    {
        return "IMPORTANT - " + super.toString();
    }

    // }}}
}
