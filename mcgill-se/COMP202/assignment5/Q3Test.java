
/**
 * Q3Test: program for testing the correct working of the NewEmail,
 *         ImportantEmail, and EmailList classes.  If these classes work
 *         correctly, then this program should print out a list of emails
 *         in appropriate prioritized order.
 **/

public class Q3Test {

    public static void main(String[] argv) {

	Email regular1 = new Email("Bob", "just an average subject", 3);
	Email regular2 = new Email("Jane", "survey", 1);
	Email regular3 = new Email("Bob", "xylophone sale!!", 3);
	Email regular4 = new Email("Jane", "info on that recipe", 12);

	NewEmail new1 = new NewEmail("Jack", "nice weather today", 2);
	NewEmail new2 = new NewEmail("Alice", "forgot my password", 1);
	NewEmail new3 = new NewEmail("Dr. Smith", "how are things?", 5);

	ImportantEmail i1 = new ImportantEmail("Dr. Smith", "emergency!", 2);
	ImportantEmail i2 = new ImportantEmail("Brian", "Homework question!", 1);
	ImportantEmail i3 = new ImportantEmail("Eve", "need password!!!", 1);

	EmailList emails = new EmailList();
	emails.add(regular1);
	emails.add(regular2);
	emails.add(regular3);
	emails.add(regular4);

	emails.add(new1);
	emails.add(new2);
	emails.add(new3);

	emails.add(i1);
	emails.add(i2);
	emails.add(i3);

	Email[] sortedList = emails.getPrioritizedList();

	for (int i=0; i < sortedList.length; i++)
	    System.out.println(sortedList[i]);
    }


} // end Q3Test class
