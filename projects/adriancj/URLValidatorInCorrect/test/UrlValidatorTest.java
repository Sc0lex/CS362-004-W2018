/* Joel Adriance
 * Kyle Wollman
 * Michael Lewis
 * 
 * CS 362 - Winter 2018
 * Final Project Part B
 * UrlValidatorTest.java
 * 
 * Description:
 * This test code is intended to expose bugs in the UrlValidator class method isValid,
 * which is intended to return true only if the supplied url string is a valid url. 
 * Also included in this file are background notes detailing our assumptions about 
 * what is and is not a valid url and areas in which we found unexpected behavior.
 */

import junit.framework.TestCase;
import java.util.ArrayList;

/* BACKGROUND - From Wikipedia (https://en.wikipedia.org/wiki/URL)
 * 
 * URL Format:
 * scheme:[//[user[:password]@]host[:port]][/path][?query][#fragment]
 * 
 * Scheme :
 * A sequence of characters beginning with a letter, followed by any combination of letters, digits, plus (+), period (.), or hyphen (-). 
 * Although schemes are case-insensitive, the canonical form is lowercase and documents that specify schemes must do so with lowercase 
 * letters. It is followed by a colon (:). Examples of popular schemes include http(s), ftp, mailto, file, data, and irc. 
 * URI schemes should be registered with the Internet Assigned Numbers Authority (IANA), although non-registered schemes are used in 
 * practice.
 * 
 * "//" : 
 *  Required by some schemes and not others. When the authority is absent, the path component cannot begin with two slashes.
 *  
 *  Authority :
 *  Three possible parts
 *  1) Optional authentication section of a user name and password, separated by a colon, followed by an at symbol (@)
 *  2) A "host", consisting of either a registered name, hostname, or an IP address. IPv4 addresses must be in dot-decimal notation, 
 *     and IPv6 addresses must be enclosed in brackets ([ ])
 *  3) Optional port number, separated from the hostname by a colon
 * 
 *  Path :
 *  Must begin with a single slash (/) following an authority. If not following an authority it begins with
 *  a slash or not, but not double slash (//). An empty path is ok.
 *  
 *  Query:
 *  Separated from preceeding part by a question mark. Delimited list of key-value pairs as
 *  <key>=<value>. W3C recommends support for both '&' and ';' as delimiters
 *  
 *  Fragment:
 *  Separated from preceeding part by #. 
 */


public class UrlValidatorTest extends TestCase { 
	
	// Expectation arrays are used both for partition testing of part validation functions
	// but, also combine to be tested in 'isValid'. 'true' indicates that the string should be
	// considered valid by the corresponding isValid function.
	
	String[] CERTAIN_SCHEMES = {"a", "http", "", "a/b/c"};

   /*************
    * TEST DATA *
    *************/  	
	// Authority tests includes port number testing
	Expectation[] AUTHORITY_TESTS = {
        new Expectation("www.google.com",  true),
	    new Expectation("go.com",          true),
	    new Expectation("go.com:80",       true), // Test port #s
	    new Expectation("go.com:65535",    true),
	    new Expectation("go.com:65636",    false),
	    new Expectation("go.com:65a",      false),
	    new Expectation("go.com:-1",       false),
	    new Expectation("go.au",           true),
	    new Expectation("0.0.0.0",         true),
	    new Expectation("255.255.255.255", true),
	    new Expectation("255.com",         true),
	    new Expectation("256.256.256.256", false),
	    new Expectation("1.2.3.4.5", false),
	    new Expectation("1.2.3.4.",  false),
	    new Expectation("1.2.3",     false),
	    new Expectation("go.a",      false),
	    new Expectation("go.a1a",    false),
	    new Expectation("go.1aa",    false),
	    new Expectation("aaa.",      false),
	    new Expectation(".aaa",      false),
	    new Expectation("aaa",       false),
	    new Expectation("",          true)};
	
    Expectation[] QUERY_TESTS = {
        new Expectation("?action=view",         true),
	    new Expectation("?action=edit&mode=up", true),
        new Expectation("",                     true),
	    new Expectation("?action=edit/mode=up", false),
	    new Expectation("??==?",                false)};
	
    Expectation[] SCHEME_TESTS_DFLT = {
        new Expectation("http",  true),
	    new Expectation("ftp",   true),
	    new Expectation("https", true),
	    new Expectation("HTTP",  true),
	    new Expectation("FTP",   true )};
	   	   
    Expectation[] SCHEME_TESTS_ALL = {
        new Expectation("http",  true),
	    new Expectation("ftp",   true),
	    new Expectation("https", true),
	    new Expectation("a",     true),
	    new Expectation("",      true),
	    new Expectation("a/b/c", true)};
   
    Expectation[] SCHEME_TESTS_CERTAIN = {
        new Expectation("a",     true),
	    new Expectation("http",  true),
        new Expectation("",      true),
        new Expectation("a/b/c", true),
        new Expectation("a/b",   false),
        new Expectation("http",  false),
        new Expectation("https", false),
        new Expectation("ftp",   false)};
	
    Expectation[] PATH_TESTS_DFLT = {
	    new Expectation("/testing123",  true),
	    new Expectation("/testing 123", false),  
	    new Expectation("/testing123/", true),  
	    new Expectation("",             true),
	    new Expectation("testing",      true),
		new Expectation("/a/b",         true),
	    new Expectation("/a(b",         true),
		new Expectation("/a&b",         true)};

    Expectation[] PATH_TESTS_DBL_SLASH = {
	    new Expectation("//testing123", true),
	    new Expectation("//a//b",       true),
	    new Expectation("a//b",         true)};	

    Expectation[] FRAG_TESTS_DFLT = {
	    new Expectation("test",           true),
	    new Expectation("",               true),
	    new Expectation("a/b/c",          true),
	    new Expectation("#testFragment",  true),
	    new Expectation("##testFragment", true),
	    new Expectation("#!@#$%^&&**()(<>?:{}testFragment", true)};

    Expectation[] FRAG_TESTS_NONE = {
        new Expectation("test",  false),
        new Expectation("",      false),
        new Expectation("a/b/c", false)};   

   public UrlValidatorTest(String testName) {
      super(testName);
   }
     
   // Test isValid with some specific urls
   public void testUrlsManually(boolean showPassed) {
   
	   Expectation[] URL_TESTS = {
	       new Expectation("http://www.google.com", true),
		   new Expectation("http://www.google.com/file1", true),
		   new Expectation("http://www.google.com/file1?action=view", true),
		   new Expectation("http://www.google.com/file1?action=view#fragment", true),
		   new Expectation("http://www.google.com:80", true),
		   new Expectation("http://www.google.com/file1/file2", true),
		   new Expectation("http:", false),
		   new Expectation("http://@#$@#$#@$@#$#$#@", false),
		   new Expectation("https://www.google.com", true),  
		   new Expectation("https://www.google.com/file1", true),
		   new Expectation("https://www.google.com/file1?action=view", true),
		   new Expectation("https://www.google.com/file1?action=view#fragment", true),
		   new Expectation("https://www.google.com:80", true),
		   new Expectation("https://www.google.com/file1/file2", true)};
   
	   Expectation[][] TEST_GROUPS = {URL_TESTS, URL_TESTS};	   
	   
	   Tester[] TESTERS =  {
	       new Tester("Default URL Validator", new UrlValidator()) {
	    	   public boolean Test(String str) {return validator.isValid(str);};},
	       
	       new Tester("All Schemes URL Validator", new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES)) {
		       public boolean Test(String str) {return validator.isValid(str);};}
	   };
	   
	   runTests("UrlValidator.isValid()", TESTERS, TEST_GROUPS, showPassed);
   }
   
   /**************
    * TEST_TOOLS *
    **************/  
   // This is a wrapper class for passing a test function (e.g. isValidAuthority) to a function that will
   // print meaningful test results and handle exceptions gracefully with an indication that they occurred.
   public abstract class Tester {
	   public String name;
	   public UrlValidator validator;
	   public Tester(String name, UrlValidator validator) {
		   this.name = name;
		   this.validator = validator;
	   }
	   abstract public boolean Test(String str); // Define the test function
   }
   
   // General purpose function that calls a test function with an expectation string, compares the result and prints a
   // meaningful result message.
   public void printTestResult(Tester tester, Expectation expectation, boolean printPassed) {
	   try {
		   if (tester.Test(expectation.input) != expectation.result)
			   System.err.println("  FAIL - " + tester.name + ": Expected '" + expectation.input + "' to be " + (expectation.result ? "valid" : "invalid"));
		   else if (printPassed)
			   System.out.println("  PASS - " + tester.name + ": '" + expectation.input + "' was " + (expectation.result ? "valid" : "invalid"));
	   }
	   catch (Throwable e) { // Catch BOTH Errors and Exceptions
		   System.err.println("  FAIL - " + tester.name + ":'" + expectation.input + "' THREW ERROR/EXCEPTION: " + e.toString());
	   }
   }
   
   // Default to ignoring passes tests for printouts
   public void printTestResult(Tester tester, Expectation expectation) {
	   printTestResult(tester, expectation, false);
   }
   
   // Build an array of testers defining test functions and a group of strings to be tested with each, then
   // call this function to report the results of tests relative to expected outcome.
   public void runTests(String testName, Tester[] testers, Expectation[][] testGroups, boolean showPassed) {
	   System.out.println("Running " + testName + " tests...");
	   for (int iGroup = 0; iGroup < testGroups.length; iGroup++) {
		   for (int iURL = 0; iURL < testGroups[iGroup].length; iURL++) {
			   printTestResult(testers[iGroup], testGroups[iGroup][iURL], showPassed);
		   }
	   }
	   System.out.println("");
   }
   
   /*************
    * FULL TEST *
    *************/  
   // The inputs are concatenated and the results are &&ed for every combination of the partition arrays
   ArrayList<Expectation> combineTestGroups(Expectation[] schemes, Expectation[] authorities, Expectation[] paths, Expectation[] queries, Expectation[] fragments) {
	   ArrayList<Expectation> combined = new ArrayList<Expectation>();
	   for (int iScheme = 0; iScheme < schemes.length; iScheme++) {
		   for (int iAuth= 0; iAuth < authorities.length; iAuth++) {
			   for (int iPath= 0; iPath < paths.length; iPath++) {
				   for (int iQuery = 0; iQuery < queries.length; iQuery++) {
					   for (int iFrag= 0; iFrag < fragments.length; iFrag++) {
						   String url = schemes[iScheme].input + "://" + authorities[iAuth].input + paths[iPath].input + queries[iQuery].input + fragments[iFrag].input;
						   boolean result = schemes[iScheme].result && authorities[iAuth].result && paths[iPath].result && queries[iQuery].result && fragments[iFrag].result;
						   combined.add(new Expectation(url, result));
					   }
				   }
			   }
		   }
	   }
	   return combined;
   }
   
   // Create complete urls by combining the constituent parts in the member arrays and test them with an appropriate validator.
   public void testIsValid(boolean showPassed)
   {   
	   // Run the special manual complete URL tests in addition to the combination of partition tests
	   testIsValidManually();
	   
	   ArrayList<Expectation> TESTS_DFLT = combineTestGroups(SCHEME_TESTS_DFLT, AUTHORITY_TESTS, PATH_TESTS_DFLT, QUERY_TESTS, FRAG_TESTS_DFLT);
	   ArrayList<Expectation> TESTS_ALL_SCHEMES = combineTestGroups(SCHEME_TESTS_ALL, AUTHORITY_TESTS, PATH_TESTS_DFLT, QUERY_TESTS, FRAG_TESTS_DFLT);
	   ArrayList<Expectation> TESTS_CERTAIN_SCHEMES = combineTestGroups(SCHEME_TESTS_CERTAIN, AUTHORITY_TESTS, PATH_TESTS_DFLT, QUERY_TESTS, FRAG_TESTS_DFLT);
	   ArrayList<Expectation> TESTS_2_SLASH_PATHS = combineTestGroups(SCHEME_TESTS_DFLT, AUTHORITY_TESTS, PATH_TESTS_DBL_SLASH, QUERY_TESTS, FRAG_TESTS_DFLT);
	   ArrayList<Expectation> TESTS_NO_FRAGS = combineTestGroups(SCHEME_TESTS_DFLT, AUTHORITY_TESTS, PATH_TESTS_DFLT, QUERY_TESTS, FRAG_TESTS_NONE);

	   Expectation[][] TEST_GROUPS = {TESTS_DFLT.toArray(new Expectation[] {}), 
			   						  TESTS_ALL_SCHEMES.toArray(new Expectation[] {}),
			   						  TESTS_CERTAIN_SCHEMES.toArray(new Expectation[] {}),
			   						  TESTS_2_SLASH_PATHS.toArray(new Expectation[] {}),
			   						  TESTS_NO_FRAGS.toArray(new Expectation[] {})};	 
	   
	   Tester[] TESTERS =  {
	       new Tester("Default URL Validator", new UrlValidator()) {
	    	   public boolean Test(String str) {return validator.isValid(str);};},
	       
	       new Tester("All Schemes URL Validator", new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES)) {
		       public boolean Test(String str) {return validator.isValid(str);};},
	    	   
	       new Tester("Certain Schemes URL Validator", new UrlValidator(CERTAIN_SCHEMES)) {
			       public boolean Test(String str) {return validator.isValid(str);};},
	       
    	   new Tester("// Path URL Validator", new UrlValidator(UrlValidator.ALLOW_2_SLASHES)) {
		       public boolean Test(String str) {return validator.isValid(str);};},
    	   
	       new Tester("No Fragments URL Validator", new UrlValidator(UrlValidator.NO_FRAGMENTS)) {
		       public boolean Test(String str) {return validator.isValid(str);};}
	   };
	   runTests("UrlValidator.isValid()", TESTERS, TEST_GROUPS, showPassed);
	   
   }
   
   /*******************
    * PARTITION TESTS *
    *******************/  
   // Test only isValidScheme (partition input to schemes only)
   public void testSchemes(boolean showPassed) {	   	     
	   Expectation[][] TEST_GROUPS = {SCHEME_TESTS_DFLT, SCHEME_TESTS_ALL, SCHEME_TESTS_CERTAIN};	   
	   
	   Tester[] TESTERS =  {
	       new Tester("Default Schemes Validator", new UrlValidator()) {
	    	   public boolean Test(String str) {return validator.isValidScheme(str);};},
	       
	       new Tester("All Schemes Validator",     new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES)) {
	           public boolean Test(String str) {return validator.isValidScheme(str);};},
	       
    	   new Tester("Certain Schemes Validator", new UrlValidator(CERTAIN_SCHEMES)) {
	    	   public boolean Test(String str) {return validator.isValidScheme(str);};}
	   };
	   
	   runTests("UrlValidator.isValidScheme()", TESTERS, TEST_GROUPS, showPassed);
   }
   
   // Test only isValidAuthority (partition input to authorities only)
   public void testAuthorities(boolean showPassed) {	 
       Expectation[][] TEST_GROUPS = {AUTHORITY_TESTS};	   
   
	   Tester[] TESTERS =  {
	       new Tester("Default Authorities Validator", new UrlValidator()) {
	    	   public boolean Test(String str) {return validator.isValidAuthority(str);};},
	   };
   
	   runTests("UrlValidator.isValidAuthority()", TESTERS, TEST_GROUPS, showPassed);
   }
   
   // Test only isValidPath (partition input to paths only)
   public void testPaths(boolean showPassed) {	   
	   Expectation[][] TEST_GROUPS = {PATH_TESTS_DFLT, PATH_TESTS_DBL_SLASH};	   
	   
	   Tester[] TESTERS =  {
	       new Tester("Default Paths Validator", new UrlValidator()) {
	    	   public boolean Test(String str) {return validator.isValidPath(str);};},
	       
    	   new Tester("// Paths Validator", new UrlValidator(UrlValidator.ALLOW_2_SLASHES)) {
	    	   public boolean Test(String str) {return validator.isValidPath(str);};}
	   };
	   
	   runTests("UrlValidator.isValidPath()", TESTERS, TEST_GROUPS, showPassed);
   }
   
   // Test only isValidQuery (partition input to queries only)
   public void testQueries(boolean showPassed) {   
	   Expectation[][] TEST_GROUPS = {QUERY_TESTS};	   
	   
	   Tester[] TESTERS =  {
	       new Tester("Default Queries Validator", new UrlValidator()) {
	    	   public boolean Test(String str) {return validator.isValidQuery(str);};},
	   };
	   
	   runTests("UrlValidator.isValidQuery()", TESTERS, TEST_GROUPS, showPassed);
   }
   
   // Test only isValidFragment (partition input to fragments only)
   public void testFragments(boolean showPassed) {	   
	   Expectation[][] TEST_GROUPS = {FRAG_TESTS_DFLT, FRAG_TESTS_NONE};	   
	   
	   Tester[] TESTERS =  {
	       new Tester("Default Frags Validator", new UrlValidator()) {
	    	   public boolean Test(String str) {return validator.isValidFragment(str);};},
	       
    	   new Tester("No Frags Validator", new UrlValidator(UrlValidator.NO_FRAGMENTS)) {
	    	   public boolean Test(String str) {return validator.isValidFragment(str);};}
	   };
	   
	   runTests("UrlValidator.isValidFragment()", TESTERS, TEST_GROUPS, showPassed);
   }

   // Test validity checks for url parts such as authority, scheme, path, etc.  
   public void testPartitions(boolean showPassed) {
	   testSchemes(showPassed);
	   testAuthorities(showPassed);
	   testPaths(showPassed);
	   testQueries(showPassed);
	   testFragments(showPassed);
   }
   
   /****************
    * MANUAL TESTS *
    ****************/  
   // Run a series of complete and partitioned URL manual tests
   public void testManually() {
	   testIsValidManually();
	   testSchemesManually();
	   testAuthoritiesManually();
	   testPathsManually();
	   testQueriesManually();
	   testFragmentsManually();
   }
     
   public void printUrlResult(String testUrl, boolean result, boolean expected)
	{
		if(result == expected)
		  {
			  System.out.println("Testing URL: " + testUrl + " Expected " + expected + " : Actual " + result);
		  }
		  
		  else
		  {
			  System.err.println("Testing URL: " + testUrl + " Expected " + expected + " : Actual " + result);
		  }
	}
	
   /* NOTES:
    * See notes for manual partition tests for details.
    * 
    * Results:
    * No Urls were valid using a UrlValidator with default arguments
  	* 'ALLOW_ALL_SCHEMES' did not work as expected in the following ways:
    * - Port number was rejected for a valid scheme and authority
    * - Valid paths are rejected
    * - Invalid URL of 'http:' was considered valid
    * - Garbage authority was accepted as valid
    */
   public void testIsValidManually() {
		  System.out.println("TESTING ALLOW_ALL_SCHEMES\n");
		  
		  UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		 
		  String testUrl = "http://www.google.com";
		  boolean result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);  
		  
		  testUrl = "http://www.google.com/file1";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http://www.google.com/file1?action=view";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http://www.google.com/file1?action=view#fragment";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http://www.google.com:80";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http://www.google.com/file1/file2";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http:";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, false);
		  
		  testUrl = "http://@#$@#$#@$@#$#$#@";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, false);
		  
		  /*testUrl = "https://www.google.com";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);  
		  
		  testUrl = "https://www.google.com/file1";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "https://www.google.com/file1?action=view";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "https://www.google.com/file1?action=view#fragment";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "https://www.google.com:80";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "https://www.google.com/file1/file2";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);*/
		  
		  System.out.println("\nTESTING DEFAULT SCHEMES\n");
		  urlVal = new UrlValidator();
		  
		  testUrl = "http://www.google.com";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);  
		  
		  testUrl = "http://www.google.com/file1";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http://www.google.com/file1?action=view";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http://www.google.com/file1?action=view#fragment";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http://www.google.com:80";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http://www.google.com/file1/file2";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "https://www.google.com";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);  
		  
		  testUrl = "https://www.google.com/file1";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "https://www.google.com/file1?action=view";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "https://www.google.com/file1?action=view#fragment";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "https://www.google.com:80";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "https://www.google.com/file1/file2";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, true);
		  
		  testUrl = "http:";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, false);
		  
		  testUrl = "http://@#$@#$#@$@#$#$#@";
		  result = urlVal.isValid(testUrl);
		  printUrlResult(testUrl, result, false);
   }
   
   /* NOTES:
    * With ALLOW_ALL_SCHEMES, any scheme should be allowed
    * Without ALLOW_ALL_SCHEMES, either the specified schemes will be checked against
    * or a null list should result in only the acceptance of the defaults that include "http", "https" and "ftp"
    * 
    * Findings: 
    * - Providing a list of acceptable schemes doesn't work. They were all rejected when tested for validity
    * - ALLOW_ALL_SCHEMES still rejects all except http 
	* - All default validator schemes are rejected
	* - Providing certain accepted schemes doesn't work. The specified schemes were not accepted
    */
   public void testSchemesManually() {
	   UrlValidator defaultValidator = new UrlValidator();
	   
	   // All of these cause a runtime exception. Something is wrong with validator when 
	   // default arguments are provided.
	   //assertTrue(defaultValidator.isValidScheme("http"));
	   //assertTrue(defaultValidator.isValidScheme("ftp"));
	   //assertTrue(defaultValidator.isValidScheme("https"));
	   //assertTrue(defaultValidator.isValidScheme("HTTP"));
	   //assertTrue(defaultValidator.isValidScheme("FTP"));	   
	    	   
	   UrlValidator allSchemesValidator = new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES);
	   
	   assertTrue(allSchemesValidator.isValidScheme("http"));
	   assertTrue(allSchemesValidator.isValidScheme("ftp"));
	   assertTrue(allSchemesValidator.isValidScheme("https"));
	   assertTrue(allSchemesValidator.isValidScheme("a"));
	   //assertTrue(allSchemesValidator.isValidScheme("")); // <-- Expected a blank scheme to be accepted
	   //assertTrue(allSchemesValidator.isValidScheme("a/b/c")); <-- This might be ok...could be an invalid formatting issue
	   
	   UrlValidator certainSchemesValidator = new UrlValidator(new String[] {"a", "http", "", "a/b/c"});
	  
	   // The provided schemes were not accepted
	   //assertTrue(certainSchemesValidator.isValidScheme("a"));
	   //assertTrue(certainSchemesValidator.isValidScheme("http"));
	   //assertTrue(certainSchemesValidator.isValidScheme(""));
	   //assertTrue(certainSchemesValidator.isValidScheme("a/b/c"));
	   assertFalse(certainSchemesValidator.isValidScheme("a/b"));
	   assertFalse(certainSchemesValidator.isValidScheme("http"));
	   assertFalse(certainSchemesValidator.isValidScheme("https"));
	   assertFalse(certainSchemesValidator.isValidScheme("ftp"));
   }
   
   public void printResultAuthority(String testAuthority, boolean result, boolean expected) {
		if(result == expected)
		  {
			  System.out.println("Testing Authority: " + testAuthority + " Expected " + expected + " : Actual " + result);
		  }
		  
		  else
		  {
			  System.err.println("Testing Authority: " + testAuthority + " Expected " + expected + " : Actual " + result);
		  }
	}
	
   /* NOTES:
    * Expect that validly formatted authorities with valid port numbers or no port numbers will be valid
    * Expect invalid port numbers to make the authority invalid
    * This test code doesn't address user authentication
    * 
    * A bug in UrlValidator causes all of these tests to fail with a runtime exception unless
    * the authority is null (not even "" works).
    */  
   public void testAuthoritiesManually() {
	   
		UrlValidator allowAllVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		String testAuthority;
		boolean result;
		
		System.out.println("\nTESTING ALLOW ALL SCHEMES\n");
		
		testAuthority = null; // This is the only thing that passes
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false); 
		
		/* ALL of these fail with default schemes
		 * Runtime exception is thrown
		testAuthority = "www.google.com";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);  

		testAuthority = "go.com";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);
		
		testAuthority = "go.com:80";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);
		
		testAuthority = "go.com:-1";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);
		
		testAuthority = "go.com:65535";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);
		
		testAuthority = "go.com:65536";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "go.au";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);

		testAuthority = "0.0.0.0";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);

		testAuthority = "255.255.255.255";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);

		testAuthority = "255.com";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);

		testAuthority = "256.256.256.256";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "1.2.3.4.5";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "1.2.3.4.";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "1.2.3";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "go.a";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "go.a1a";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "go.1aa";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "aaa.";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = ".aaa";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "aaa";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "";
		result = allowAllVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);
		*/
		System.out.println("\nTESTING DEFAULT SCHEMES\n");
		
		UrlValidator defaultVal = new UrlValidator();

		testAuthority = null; // This is the only thing that passes
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false); 
		
		/* ALL of these fail also
		 * Runtime exception is thrown
		testAuthority = "www.google.com";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);  

		testAuthority = "go.com";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);

		testAuthority = "go.au";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);

		testAuthority = "0.0.0.0";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);

		testAuthority = "255.255.255.255";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);

		testAuthority = "255.com";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, true);

		testAuthority = "256.256.256.256";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "1.2.3.4.5";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "1.2.3.4.";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "1.2.3";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "go.a";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "go.a1a";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "go.1aa";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "aaa.";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = ".aaa";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "aaa";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);

		testAuthority = "";
		result = defaultVal.isValidAuthority(testAuthority);
		printResultAuthority(testAuthority, result, false);
  		*/
   }
   
   /* NOTES:
    * Paths with double slash '//' should be accepted if ALLOW_2_SLASHES is set. Sub-directories should be allowed
    * 
    * Findings: 
    * - Paths with sub-directories fail
    * - The ALLOW_2_SLASHES flag does not cause paths with // to be accepted
    * - Trailing slashes are rejected and probably shouldn't be generally
    */
   public void testPathsManually() {
	   UrlValidator defaultValidator = new UrlValidator();
	   
	   assertTrue(defaultValidator.isValidPath("/testing123"));
	   assertFalse(defaultValidator.isValidPath("/testing 123"));
	   //assertTrue(defaultValidator.isValidPath("/testing123/"));
	   assertTrue(defaultValidator.isValidPath(""));
	   //assertTrue(defaultValidator.isValidPath("testing"));
	   //assertTrue(defaultValidator.isValidPath("/a/b"));
	   assertTrue(defaultValidator.isValidPath("/a(b"));
	   assertTrue(defaultValidator.isValidPath("/a&b"));

	   UrlValidator doubleSlashValidator = new UrlValidator();
	    
	   //assertTrue(doubleSlashValidator.isValidPath("//testing123"));
	   //assertTrue(doubleSlashValidator.isValidPath("//a//b"));
	   //assertTrue(doubleSlashValidator.isValidPath("a//b"));
   }
   
   public void printResultQuery(String testQuery, boolean result, boolean expected)
	{
		if(result == expected)
		  {
			  System.out.println("Testing Query: " + testQuery + " Expected " + expected + " : Actual " + result);
		  }
		  
		  else
		  {
			  System.err.println("Testing Query: " + testQuery + " Expected " + expected + " : Actual " + result);
		  }
	}
   
  
   /* NOTES:
    * Expect that only validly formated queries and empty queries will be valid.
    * A valid query should start with '?' and include name=value pairs separated by &
    * 
    * Actual Result:
    * Seems to accept just about any string as a valid query except one with a space
    */   
   public void testQueriesManually() {
		UrlValidator allowAllVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

		String testQuery = "?action=view";
		boolean result = allowAllVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, true);

		testQuery = "?action=edit&mode=up";
		result = allowAllVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, true);
		
		testQuery = "?action=edit&mo de=up"; // <-- Space not allowed
		result = allowAllVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, false);

		testQuery = "";
		result = allowAllVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, true);

		testQuery = "?action=edit/mode=up";
		result = allowAllVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, false);

		testQuery = "??==?";
		result = allowAllVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, false);
		/*
		System.out.println("\nTESTING DEFAULT SCHEMES\n");
		UrlValidator defaultVal = new UrlValidator();

		testQuery = "?action=view";
		result = defaultVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, true);

		testQuery = "?action=edit&mode=up";
		result = defaultVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, true);

		testQuery = "";
		result = defaultVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, true);

		testQuery = "?action=edit/mode=up";
		result = defaultVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, false);

		testQuery = "??==?";
		result = defaultVal.isValidQuery(testQuery);
		printResultQuery(testQuery, result, false);
		 */
   }
   
   /* NOTES:
    * Not sure what the real rules are for fragments, but I wouldn't think that an empty fragment is ok...
    * 
    * Findings: 
    * - All fragments tested were accepted, even an empty fragment, one with spaces, and one starting with '#'. 
    * - Setting the NO_FRAGMENTS flag appears to work correctly in that it did reject all fragments tested
    */
   public void testFragmentsManually() {
	   UrlValidator defaultValidator = new UrlValidator();
	   
	   assertTrue(defaultValidator.isValidFragment("test"));
	   //assertFalse(defaultValidator.isValidFragment("")); / <-- It actually does accept empty fragments... maybe ok?
	   assertTrue(defaultValidator.isValidFragment("a/b/c"));
	   assertTrue(defaultValidator.isValidFragment("#testFragment"));
	   //assertFalse(defaultValidator.isValidFragment("#test Fragment")); <-- Probaby shouldn't accept this
	   assertTrue(defaultValidator.isValidFragment("##testFragment"));
	   assertTrue(defaultValidator.isValidFragment("#!@#$%^&&**()(<>?:{}testFragment"));

	   UrlValidator noFragsValidator = new UrlValidator(UrlValidator.NO_FRAGMENTS);
	   assertFalse(noFragsValidator.isValidFragment("test"));
	   assertFalse(noFragsValidator.isValidFragment(""));
	   assertFalse(noFragsValidator.isValidFragment("a/b/c"));  
   }
   
   /********************
    * MAIN ENTRY POINT *
    ********************/
   // Execute test
   public static void main(String[] argv) {

	   UrlValidatorTest tester = new UrlValidatorTest("URLValidator Test");
	   System.out.println("Running manual tests...");
	   tester.testManually(); // Individually tested urls or url parts
	   
	   System.out.println("Running partition tests...");
	   tester.testPartitions(false); // Automated tests of url parts (e.g. scheme, authority, path, etc.)
	   
	   System.out.println("Running URL isValid tests...");
	   tester.testIsValid(false); // Combine and test complete urls via the 'isValid' function
   }
}
