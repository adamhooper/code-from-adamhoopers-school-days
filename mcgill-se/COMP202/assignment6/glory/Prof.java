// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 6
// Developed at: Home
//
// Prof: Stores a professor

public class Prof extends People
{
    private int rating;
    private double salary;

    // {{{ constructor
    /**
     * Creates a new Prof object.
     *
     * @param firstName First name.
     * @param lastName  Last name.
     * @param id        ID.
     * @param rating    Rating (1-5)
     * @param salary    Salary.
     */
    public Prof(String firstName, String lastName, int id, int rating,
                double salary)
    {
        super(firstName, lastName, id);
        super.setType('P');

        this.rating = rating;
        this.salary = salary;
    }

    // }}}
    // {{{ setRating()
    /**
     * Sets the person's rating.
     *
     * @param rating The person's rating.
     */
    public void setRating(int rating)
    {
        this.rating = rating;
    }

    // }}}
    // {{{ getRating()
    /**
     * Returns the person's rating.
     *
     * @return the person's rating.
     */
    public int getRating()
    {
        return this.rating;
    }

    // }}}
    // {{{ setSalary()
    /**
     * Sets the person's salary.
     *
     * @param salary The person's salary.
     */
    public void setSalary(double salary)
    {
        this.salary = salary;
    }

    // }}}
    // {{{ getSalary()
    /**
     * Returns the person's salary.
     *
     * @return the person's salary.
     */
    public double getSalary()
    {
        return this.salary;
    }

    // }}}
    // {{{ toString()
    public String toString()
    {
        return super.toString() + "\n"
               + "Rating: " + getRating() + "\n"
               + "Salary: " + getSalary();
    }

    // }}}
}
