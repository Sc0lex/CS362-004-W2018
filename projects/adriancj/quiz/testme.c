/* Joel Adriance
 * CS 362 0 Winter 2018
 * Random Testing Quiz
 *
 * Implemented inputChar and inputString random
 * generators per the assignment instructions.
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define STRING_LEN 5
char g_str[STRING_LEN + 1];

char inputChar()
{
    // TODO: rewrite this function
	// Select a random character from all 256 ASCII chars
	return (char)(rand() % 94 + 32);
}

char *inputString()
{
    // TODO: rewrite this function
	// Return a random string by returning a random sequence of chars
	memset(g_str, 0, STRING_LEN + 1);
	int i;
	for (i = 0; i < STRING_LEN; ++i) {
		//g_str[i] = inputChar();
		g_str[i] = (char)(rand() % 26 + 97);
	}
    return g_str;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
	  exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
