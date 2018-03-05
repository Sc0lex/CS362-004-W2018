# Modified by Joel Adriance 2-3-2018
# now provides operation for handling unit tests

#CFLAGS = -O0 -Wall -fpic -coverage -lm

CFLAGS = -O0 -Wall -fpic -coverage -fprofile-arcs -lm	
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
	gcc -c testUtils.c -g $(CFLAGS)

unittest1: unittest1.c testUtils.o dominion.o rngs.o
	gcc -o unittest1 unittest1.c testUtils.o dominion.o rngs.o -g $(CFLAGS)
unittest2: unittest2.c testUtils.o dominion.o rngs.o
	gcc -o unittest2 unittest2.c testUtils.o dominion.o rngs.o -g $(CFLAGS)
unittest3: unittest3.c testUtils.o dominion.o rngs.o
	gcc -o unittest3 unittest3.c testUtils.o dominion.o rngs.o -g $(CFLAGS) 
unittest4: unittest4.c testUtils.o dominion.o rngs.o
	gcc -o unittest4 unittest4.c testUtils.o dominion.o rngs.o -g $(CFLAGS)
cardtest1: cardtest1.c testUtils.o dominion.o rngs.o
	gcc -o cardtest1 cardtest1.c testUtils.o dominion.o rngs.o -g $(CFLAGS) 
cardtest2: cardtest2.c testUtils.o dominion.o rngs.o
	gcc -o cardtest2 cardtest2.c testUtils.o dominion.o rngs.o -g $(CFLAGS)
cardtest3: cardtest3.c testUtils.o dominion.o rngs.o
	gcc -o cardtest3 cardtest3.c testUtils.o dominion.o rngs.o -g $(CFLAGS)
cardtest4: cardtest4.c testUtils.o dominion.o rngs.o
	gcc -o cardtest4 cardtest4.c testUtils.o dominion.o rngs.o -g $(CFLAGS)
randomtestadventurer: randomtestadventurer.c testUtils.o dominion.o rngs.o
	gcc -o randomtestadventurer randomtestadventurer.c testUtils.o dominion.o rngs.o -g $(CFLAGS) 
randomtestcard1: randomtestcard1.c testUtils.o dominion.o rngs.o
	gcc -o randomtestcard1 randomtestcard1.c testUtils.o dominion.o rngs.o -g $(CFLAGS)
randomtestcard2: randomtestcard2.c testUtils.o dominion.o rngs.o
	gcc -o randomtestcard2 randomtestcard2.c testUtils.o dominion.o rngs.o -g $(CFLAGS)

unittest1.out: unittest1
	echo ""                                    >  unittest1.out
	echo "--- Unit Test 1 Results ---"         >> unittest1.out
	./unittest1                                >> unittest1.out
	echo ""                                    >> unittest1.out
	echo "--- Unit Test 1 Coverage ---"        >> unittest1.out
	gcov -f -b dominion.c                      >> unittest1.out
unittest2.out: unittest2
	echo ""                                    >  unittest2.out
	echo "--- Unit Test 2 Results ---"         >> unittest2.out
	./unittest2                                >> unittest2.out
	echo ""                                    >> unittest2.out
	echo "--- Unit Test 2 Coverage ---"        >> unittest2.out
	gcov -f -b dominion.c                      >> unittest2.out
unittest3.out: unittest3
	echo ""                                    >  unittest3.out
	echo "--- Unit Test 3 Results ---"         >> unittest3.out
	./unittest3                                >> unittest3.out
	echo ""                                    >> unittest3.out
	echo "--- Unit Test 3 Coverage ---"        >> unittest3.out
	gcov -f -b dominion.c                      >> unittest3.out
unittest4.out: unittest4
	echo ""                                    >  unittest4.out
	echo "--- Unit Test 4 Results ---"         >> unittest4.out
	./unittest4                                >> unittest4.out
	echo ""                                    >> unittest4.out
	echo "--- Unit Test 4 Coverage ---"        >> unittest4.out
	gcov -f -b dominion.c                      >> unittest4.out
cardtest1.out: cardtest1
	echo ""                                    >  cardtest1.out
	echo "--- Card Test 1 Results ---"         >> cardtest1.out
	./cardtest1                                >> cardtest1.out
	echo ""                                    >> cardtest1.out
	echo "--- Card Test 1 Coverage ---"        >> cardtest1.out
	gcov -f -b dominion.c                      >> cardtest1.out
cardtest2.out: cardtest2
	echo ""                                    >  cardtest2.out
	echo "--- Card Test 2 Results ---"         >> cardtest2.out
	./cardtest2                                >> cardtest2.out
	echo ""                                    >> cardtest2.out
	echo "--- Card Test 2 Coverage ---"        >> cardtest2.out
	gcov -f -b dominion.c                      >> cardtest2.out
cardtest3.out: cardtest3
	echo ""                                    >  cardtest3.out
	echo "--- Card Test 3 Results ---"         >> cardtest3.out
	./cardtest3                                >> cardtest3.out
	echo ""                                    >> cardtest3.out
	echo "--- Card Test 3 Coverage ---"        >> cardtest3.out
	gcov -f -b dominion.c                      >> cardtest3.out
cardtest4.out: cardtest4
	echo ""                                    >  cardtest4.out
	echo "--- Card Test 4 Results ---"         >> cardtest4.out
	./cardtest4                                >> cardtest4.out
	echo ""                                    >> cardtest4.out
	echo "--- Card Test 4 Coverage ---"        >> cardtest4.out
	gcov -f -b dominion.c                      >> cardtest4.out
randomtestcard1.out: randomtestcard1
	echo ""                                    >  randomtestcard1.out
	echo "--- Random Test Card 1 Results ---"  >> randomtestcard1.out
	./randomtestcard1                          >> randomtestcard1.out
	echo ""                                    >> randomtestcard1.out
	echo "--- Random Test Card 1 Coverage ---" >> randomtestcard1.out
	gcov -f -b dominion.c                      >> randomtestcard1.out
randomtestcard2.out: randomtestcard2
	echo ""                                    >  randomtestcard2.out
	echo "--- Random Test Card 2 Results ---"  >> randomtestcard2.out
	./randomtestcard2                          >> randomtestcard2.out
	echo ""                                    >> randomtestcard2.out
	echo "--- Random Test Card 2 Coverage ---" >> randomtestcard2.out
	gcov -f -b dominion.c                      >> randomtestcard2.out
randomtestadventurer.out: randomtestadventurer
	echo ""                                        >  randomtestadventurer.out
	echo "--- Random Test Adventurer Results ---"  >> randomtestadventurer.out
	./randomtestadventurer                         >> randomtestadventurer.out
	echo ""                                        >> randomtestadventurer.out
	echo "--- Random Test Adventurer Coverage ---" >> randomtestadventurer.out
	gcov -f -b dominion.c                          >> randomtestadventurer.out

unittestresults: unittest1.out unittest2.out unittest3.out unittest4.out
cardtestresults: cardtest1.out cardtest2.out cardtest3.out cardtest4.out
randomtestresults: randomtestadventurer.out randomtestcard1.out randomtestcard2.out

alltestresults: unittestresults cardtestresults randomtestresults

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
