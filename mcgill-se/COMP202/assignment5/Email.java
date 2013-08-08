
/**
 * Email: class that represents a regular email.  An regular email's priority
 *        is the same as its age (so given an age of 4, an regular email has
 *        priority 4)
 **/

public class Email {

    private String sender;
    private String subject;
    private int age;


    // Constructor: creates a blank email (no information).  This email has
    // a priority of -1
    public Email() {
	sender = "";
	subject = "";
	age = -1;
    }


    // Constructor: creates an email with a given sender, subject, and
    // age (in terms of days old).
    public Email(String sender, String subject, int ageInDays) {
	this.sender = sender;
	this.subject = subject;
	this.age = ageInDays;
    }


    public void setSender(String s) {
	sender = s;
    }

    public String getSender() {
	return sender;
    }

    public void setSubject(String s) {
	subject = s;
    }

    public String getSubject() {
	return subject;
    }

    public void setAge(int days) {
	age = days;
    }

    public int getAge() {
	return age;
    }



    // Calculates and returns the priority for this email.  Regular emails
    // have priority the same as their age.
    public int getPriority() {
	return age;
    }



    // For use in System.out.println commands on this object.  Given a subject
    // "SUBJECT" sender "SENDER" and age ##, this method will return the
    // string 'From: SENDER, Subject: SUBJECT, ## days old"

    public String toString() {
	String answer;
	answer = "From: " + sender + ", Subject: " + subject + ", "
	         + age + " days old";

	return answer;
    }


} // end Email class
