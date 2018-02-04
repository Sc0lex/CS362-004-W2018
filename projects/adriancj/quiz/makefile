# Modified by Joel Adriance 2-3-2018
# now provides operation for handling unit tests

CFLAGS = -Wall -fpic -coverage -lm
	
testme: testme.c
	gcc -o testme testme.c -g  $(CFLAGS)
	testme
	
all: testme

clean:
	rm -f *.o *.gcov *.gcda *.gcno *.so *.out
