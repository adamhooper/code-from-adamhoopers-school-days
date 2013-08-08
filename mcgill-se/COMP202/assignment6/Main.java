// Name: Adam Hooper
// McGill ID: 260055737
// Course: COMP202
// Section: 002
// Assignment: 6
// Developed at: Home
//
// Main: Program to read and manipulate a database of people in people.txt

import cs1.Keyboard;
import java.util.StringTokenizer;
import java.io.*;

public class Main
{
    private static People[] school = new People[100];
    private static int school_length = 0;

    // {{{ main()
    /**
     * The main program.
     */
    public static void main(String[] argv) throws IOException
    {
        loadSchool();

        doLastNameInput();
    }

    // }}}
    // {{{ doLastNameInput()
    /**
     * Asks a user for a last name, then lets him process the person associated.
     */
    private static void doLastNameInput()
    {
        String input;
        People person;
        int index;

        while (true) {
            System.out.println();
            System.out.print("Enter last name of person to edit, or DONE to "
                             + "exit: ");

            input = Keyboard.readString();

            if (input.equals("DONE")) {
                System.out.println("Exiting.");
                break;
            }

            index = findLastName(input);

            if (index == -1) {
                System.out.println("Person not found. Please try again.");
                continue;
            }

            person = school[index];

            doProcess(person);
        }
    }

    // }}}
    // {{{ doProcess()
    /**
     * Lets the user process the person specified.
     *
     * @param person The person to process
     */
    private static void doProcess(People person)
    {
        int input;

        do {
            System.out.println("Processing:");
            System.out.println(person);
            System.out.println();

            switch (person.getType()) {
                case 'P':
                    System.out.println("1. Edit rating");
                    System.out.println("2. Edit salary");
                    break;
                case 'S':
                    System.out.println("1. Edit GPA");
                    System.out.println("2. Edit full-time status");
                    break;
                default:
                    System.out.println("This menu is broken!");
            }

            System.out.println("0. Exit");
            System.out.print("Enter your choice: ");

            input = Keyboard.readInt();

            switch (person.getType()) {
                case 'P':
                    if (input == 1)
                        editProfRating((Prof) person);
                    if (input == 2)
                        editProfSalary((Prof) person);
                    break;
                case 'S':
                    if (input == 1)
                        editStudentGpa((Student) person);
                    if (input == 2)
                        editStudentFullTime((Student) person);
                    break;
                default:
                    System.out.println("Invalid selection.");
                    System.out.println();
            }

        } while (input != 0);
    }

    // }}}
    // {{{ findLastName()
    /**
     * Returns school's index for the person with the given last name.
     *
     * @param s lastName The person's last name.
     * @return  The index of that person, or -1 if not found.
     */
    private static int findLastName(String s)
    {
        for (int i = 0; i < school_length; i++) {
            if (s.equals(school[i].getLastName()))
                return i;
        }

        return -1;
    }

    // }}}
    // {{{ editProfRating()
    /**
     * Allows the user to edit the given prof's rating.
     *
     * Will prompt for integers from 1 to 5 until an invalid value is given;
     * the professor will receive the average of these ratings.
     *
     * @param p Prof to edit.
     */
    private static void editProfRating(Prof p)
    {
        int sum = 0;
        int count = 0;
        int input;

        System.out.println("Enter some digits from 1-5.");
        System.out.println("Enter any other number to exit.");

        while ((input = Keyboard.readInt()) > 0 && input < 6) {
            sum += input;
            count++;
        }

        if (count == 0)
            p.setRating(0);
        else
            p.setRating(sum/count);

        System.out.println("Set new rating to " + p.getRating() + ".");
    }

    // }}}
    // {{{ editProfSalary()
    /**
     * Asks for a new salary and gives it to the passed prof.
     *
     * @param p Professor whose salary we will update.
     */
    private static void editProfSalary(Prof p)
    {
        System.out.print("Enter a new salary: ");

        p.setSalary(Keyboard.readDouble());

        System.out.println("Salary set to " + p.getSalary() + ".");
    }

    // }}}
    // {{{ editStudentGpa()
    /**
     * Asks for a GPA and gives it to the passed Student.
     *
     * @param s Student who will receive a new GPA.
     */
    private static void editStudentGpa(Student s)
    {
        boolean success;

        do {
            System.out.print("Please enter a GPA between 0.0 and 4.0: ");

            success = s.setGpa(Keyboard.readDouble());
        } while (!success);

        System.out.println("GPA set to " + s.getGpa() + ".");
    }

    // }}}
    // {{{ editStudentFullTime()
    /**
     * Asks the user whether or not he wants to toggle the full-time status.
     *
     * @param s Student whose status we want to change.
     */
    private static void editStudentFullTime(Student s)
    {
        char input;

        System.out.println("Current status: " + s.getFullTimeString() + ".");
        System.out.print("Are you sure you want to change this? (Y/N) ");

        input = Keyboard.readChar();

        if (input == 'Y' || input == 'y')
            s.setFullTime(!s.getFullTime());

        System.out.println("Status: " + s.getFullTimeString() + ".");
    }

    // }}}
    // {{{ loadSchool()
    /**
     * Loads people.txt into the private school variable.
     */
    private static void loadSchool() throws IOException, FileNotFoundException
    {
        String s;
        int i;

        FileReader fr = new FileReader("people.txt");
        BufferedReader br = new BufferedReader(fr);

        while ((s = br.readLine()) != null) {
            school[school_length++] = parsePerson(s);
        }
    }

    // }}}
    // {{{ parsePerson()
    /**
     * Returns an object representation of the person given as a string.
     *
     * @param s String representation of a person.
     * @return  A representation as a People object.
     */
    private static People parsePerson(String s)
    {
        StringTokenizer st = new StringTokenizer(s);

        String lastName = st.nextToken();
        String firstName = st.nextToken();
        int id = Integer.parseInt(st.nextToken());
        char type = st.nextToken().charAt(0);

        switch (type) {
            case 'S':
                double gpa = Double.parseDouble(st.nextToken());
                boolean fullTime = st.nextToken().equals("T");
                
                return new Student(firstName, lastName, id, gpa, fullTime);
            case 'P':
                int rating = Integer.parseInt(st.nextToken());
                double salary = Double.parseDouble(st.nextToken());

                return new Prof(firstName, lastName, id, rating, salary);
        }

        System.out.println("Invalid data file!");
        return new People(firstName, lastName, id);
    }

    // }}}
}
