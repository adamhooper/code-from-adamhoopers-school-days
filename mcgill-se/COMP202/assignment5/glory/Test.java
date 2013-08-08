import cs1.Keyboard;
import java.util.StringTokenizer;

public class Test {
    public static void main(String[] argv) {
        int i = 0;
        int[] list;
        String input;
        StringTokenizer tokenizer;
        EqualSplitter splitter = new EqualSplitter();

        System.out.println("Enter a comma-separated list of integers:");
        input = Keyboard.readString();

        tokenizer = new StringTokenizer(input, ", ");

        list = new int[tokenizer.countTokens()];

        while (tokenizer.hasMoreTokens())
            list[i++] = Integer.parseInt(tokenizer.nextToken());

        splitter.equalSplit(list);
    }
}
