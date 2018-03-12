
// Holds an input string that will be checked for validity and 
// a result value indicating whether it should be considered
// valid.
public class Expectation {
	public String  input;
	public boolean result;
	public Expectation(String input, boolean result) {
		this.input = input;
		this.result = result;
	}
}