// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 5
// Developed at: Home
//
// NewEmail: Take a wild guess.

public class NewEmail extends Email
{
    // {{{ constructor
    /**
     * Create a new NewEmail object.
     */
    public NewEmail() { super(); }

    public NewEmail(String sender, String subject, int ageInDays)
    {
        super(sender, subject, ageInDays);
    }

    // }}}
    // {{{ getPriority()
    public int getPriority()
    {
        return 3 * getAge();
    }

    // }}}
    // {{{ toString()
    /**
     * Puts 'NEW' before the default <code>Email</code> string representation.
     *
     * @return String representation of this Email.
     */
    public String toString()
    {
        return "NEW - " + super.toString();
    }

    // }}}
}
