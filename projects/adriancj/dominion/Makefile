# Modified by Joel Adriance 2-3-2018
# now provides operation for handling unit tests

CFLAGS = -O0 -Wall -fpic -coverage -lm
	
rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)
	

# Unit test rules for Assignment 3 - Joel Adriance
testUtils.o: testUtils.c testUtils.h
	gcc -c testUtils.c

unittest1: unittest1.c testUtils.o dominion.o rngs.o
	gcc -o unittest1 unittest1.c testUtils.o dominion.o rngs.o $(CFLAGS)
unittest2: unittest2.c testUtils.o dominion.o rngs.o
	gcc -o unittest2 unittest2.c testUtils.o dominion.o rngs.o $(CFLAGS)
unittest3: unittest3.c testUtils.o dominion.o rngs.o
	gcc -o unittest3 unittest3.c testUtils.o dominion.o rngs.o $(CFLAGS) 
unittest4: unittest4.c testUtils.o dominion.o rngs.o
	gcc -o unittest4 unittest4.c testUtils.o dominion.o rngs.o $(CFLAGS)
	
cardtest1: cardtest1.c testUtils.o dominion.o rngs.o
	gcc -o cardtest1 cardtest1.c testUtils.o dominion.o rngs.o $(CFLAGS) 
cardtest2: cardtest2.c testUtils.o dominion.o rngs.o
	gcc -o cardtest2 cardtest2.c testUtils.o dominion.o rngs.o $(CFLAGS)
cardtest3: cardtest3.c testUtils.o dominion.o rngs.o
	gcc -o cardtest3 cardtest3.c testUtils.o dominion.o rngs.o $(CFLAGS)
cardtest4: cardtest4.c testUtils.o dominion.o rngs.o
	gcc -o cardtest4 cardtest4.c testUtils.o dominion.o rngs.o $(CFLAGS)
	
unittests: unittest1 unittest2 unittest3 unittest4
cardtests: cardtest1 cardtest2 cardtest3 cardtest4

unittestresults.out: unittests cardtests 
	echo "" 							>  unittestresults.out
	echo "--- Unit Test 1 Results ---"  >> unittestresults.out
	unittest1 							>> unittestresults.out
	echo "" 							>> unittestresults.out
	echo "--- Unit Test 2 Results ---"  >> unittestresults.out
	unittest2 							>> unittestresults.out
	echo "" 							>> unittestresults.out
	echo "--- Unit Test 3 Results ---"  >> unittestresults.out
	unittest3 							>> unittestresults.out
	echo "" 							>> unittestresults.out
	echo "--- Unit Test 4 Results ---"  >> unittestresults.out
	unittest4 							>> unittestresults.out
	echo "" 							>> unittestresults.out
	echo "--- Card Test 1 Results ---"  >> unittestresults.out
	cardtest1 							>> unittestresults.out
	echo "" 							>> unittestresults.out
	echo "--- Card Test 2 Results ---"  >> unittestresults.out
	cardtest2 							>> unittestresults.out
	echo "" 							>> unittestresults.out
	echo "--- Card Test 3 Results ---"  >> unittestresults.out
	cardtest3 							>> unittestresults.out
	echo "" 							>> unittestresults.out
	echo "--- Card Test 4 Results ---"  >> unittestresults.out
	cardtest4 							>> unittestresults.out
	echo "" 							>> unittestresults.out
	echo "--- Unit Test Coverage ---"   >> unittestresults.out
	gcov -f -b dominion.c 				>> unittestresults.out	
	
all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
