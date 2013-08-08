// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 6
// Developed at: Home
//
// Student: Stores a student

public class Student extends People
{
    private double GPA;
    private boolean FullTime;

    // {{{ constructor
    /**
     * Creates a new Student.
     *
     * @param firstName First name.
     * @param lastName  Last name.
     * @param id        ID.
     * @param gpa       GPA.
     * @param fullTime  Whether the student is full-time.
     */
    public Student(String firstName, String lastName, int id, double gpa,
                   boolean fullTime)
    {
        super(firstName, lastName, id);
        super.setType('S');

        this.GPA = gpa;
        this.FullTime = fullTime;
    }

    // }}}
    // {{{ setGpa()
    /**
     * Sets the person's GPA.
     *
     * GPA will be set to the average between the given GPA and the existing
     * one.
     *
     * @param gpa The person's GPA.
     * @return    Whether the command was successful.
     */
    public boolean setGpa(double gpa)
    {
        if (gpa < 0 || gpa > 4)
            return false;

        this.GPA = (gpa + this.GPA) / 2;
        return true;
    }

    // }}}
    // {{{ getGpa()
    /**
     * Returns the person's GPA.
     *
     * @return the person's GPA.
     */
    public double getGpa()
    {
        return this.GPA;
    }

    // }}}
    // {{{ setFullTime()
    /**
     * Sets the person's full-time status.
     *
     * @param fullTime The person's full-time status.
     */
    public void setFullTime(boolean fullTime)
    {
        this.FullTime = fullTime;
    }

    // }}}
    // {{{ getFullTime()
    /**
     * Returns the person's full-time status.
     *
     * @return the person's full-time status.
     */
    public boolean getFullTime()
    {
        return this.FullTime;
    }

    // }}}
    // {{{ getFullTimeString()
    /**
     * Returns the person's full-time status as a string.
     *
     * @return the person's full-time status.
     */
    public String getFullTimeString()
    {
        if (this.getFullTime())
            return "full-time";

        return "part-time";
    }

    // }}}
    // {{{ toString()
    public String toString()
    {
        return super.toString() + "\n"
               + "GPA: " + getGpa() + "\n"
               + "Full-time status: " + getFullTimeString();
    }

    // }}}
}
