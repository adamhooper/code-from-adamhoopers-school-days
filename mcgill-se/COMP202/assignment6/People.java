// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 6
// Developed at: Home
//
// People: Stores a single person

public class People
{
    private String FirstName, LastName;
    private int ID;
    private char Type;

    // {{{ constructor
    /**
     * Creates a new People object.
     *
     * @param firstName First name.
     * @param lastName  Last name.
     * @param id        ID.
     */
    public People(String firstName, String lastName, int id)
    {
        this.FirstName = firstName;
        this.LastName = lastName;
        this.ID = id;
    }
    // }}}
    // {{{ setFirstName()
    /**
     * Sets the person's first name.
     *
     * @param name The person's first name.
     */
    public void setFirstName(String name)
    {
        this.FirstName = name;
    }

    // }}}
    // {{{ getFirstName()
    /**
     * Returns the person's first name.
     *
     * @return The person's first name.
     */
    public String getFirstName()
    {
        return this.FirstName;
    }

    // }}}
    // {{{ setLastName()
    /**
     * Sets the person's last name.
     *
     * @param name The person's last name.
     */
    public void setLastName(String name)
    {
        this.LastName = name;
    }

    // }}}
    // {{{ getLastName()
    /**
     * Returns the person's last name.
     *
     * @return The person's last name.
     */
    public String getLastName()
    {
        return this.LastName;
    }

    // }}}
    // {{{ setId()
    /**
     * Sets the person's ID.
     *
     * @param id The person's ID.
     */
    public void setId(int id)
    {
        this.ID = id;
    }

    // }}}
    // {{{ getId()
    /**
     * Returns the person's ID.
     *
     * @return the person's ID.
     */
    public int getId()
    {
        return this.ID;
    }

    // }}}
    // {{{ setType()
    /**
     * Sets this person's type.
     *
     * 'P' means professor; 'S' means student.
     *
     * @param type The person's type.
     */
    protected void setType(char type)
    {
        this.Type = type;
    }

    // }}}
    // {{{ getType()
    /**
     * Returns the person's type.
     *
     * 'P' means professor; 'S' means student.
     *
     * @return the person's type.
     */
    public char getType()
    {
        return this.Type;
    }

    // }}}
    // {{{ toString()
    /**
     * Returns a String representation of the person.
     *
     * @return String representation of the person.
     */
    public String toString()
    {
        return "Name: " + getFirstName() + " " + getLastName() + "\n"
               + "ID: " + getId();
    }

    // }}}
}
