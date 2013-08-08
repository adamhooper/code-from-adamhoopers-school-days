// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 5
// Developed at: Home
//
// EmailList: List for sorting a bunch of Emails

public class EmailList
{
    private Email[] emails;
    private int nextEmail;
    private final int MAX_EMAILS = 200;

    // {{{ constructor()
    /**
     * Creates a new, empty EmailList.
     */
    public EmailList()
    {
        emails = new Email[MAX_EMAILS];
        nextEmail = 0;
    }

    // }}}
    // {{{ add()
    /**
     * Adds an email to the list.
     *
     * @param Email to add.
     */
    public void add(Email e)
    {
        if (e == null)
            return;

        emails[nextEmail++] = e;
    }

    // }}}
    // {{{ getPrioritizedList()
    /**
     * Returns an <code>Array</code> of <code>Email</code>s in order.
     *
     * Sort by: priority, age, sender (case-insensitive), subject.
     *
     * @return <code>Array</code> of <code>Email</code>s.
     */
    public Email[] getPrioritizedList()
    {
        Email[] ret;
        int i;

        sort();

        ret = new Email[nextEmail];

        for (i = 0; i < ret.length; i++)
            ret[i] = emails[i];

        return ret;
    }

    // }}}

    // {{{ sort()
    /**
     * Sorts the internal list of <code>Email</code>s.
     */
    private void sort()
    {
        int i, j, minPos;
        Email t;

        // Selection sort
        for (i = 0; i < nextEmail; i++) {
            minPos = i;
            for (j = i; j < nextEmail; j++) {
                if (compareEmails(emails[j], emails[minPos]) < 0)
                    minPos = j;
            }
            // Swap
            t = emails[i];
            emails[i] = emails[minPos];
            emails[minPos] = t;
        }
    }

    // }}}
    // {{{ compareEmails()
    /**
     * Compares two emails.
     *
     * Compares by: priority, age, sender (case-insensitive), subject.
     *
     * @return less than 0 if the first is higher-priority than the second;
     *         0           if they are identical;
     *         more than 0 if the second is higher-priority than the first.
     */
    private int compareEmails(Email a, Email b)
    {
        int t;

        t = b.getPriority() - a.getPriority();
        if (t != 0)
            return t;

        t = b.getAge() - a.getAge();
        if (t != 0)
            return t;

        t = a.getSender().compareToIgnoreCase(b.getSender());
        if (t != 0)
            return t;

        t = a.getSubject().compareToIgnoreCase(b.getSender());
        if (t != 0)
            return t;

        return 0;
    }

    // }}}
}
