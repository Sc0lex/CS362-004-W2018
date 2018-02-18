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
	gcc -c testUtils.c $(CFLAGS)

randomtestadventurer: randomtestadventurer.c testUtils.o dominion.o rngs.o
	gcc -o randomtestadventurer randomtestadventurer.c testUtils.o dominion.o rngs.o $(CFLAGS) 
randomtestcard1: randomtestcard1.c testUtils.o dominion.o rngs.o
	gcc -o randomtestcard1 randomtestcard1.c testUtils.o dominion.o rngs.o $(CFLAGS)
randomtestcard2: randomtestcard2.c testUtils.o dominion.o rngs.o
	gcc -o randomtestcard2 randomtestcard2.c testUtils.o dominion.o rngs.o $(CFLAGS)
	
cardtests: randomtestadventurer randomtestcard1 randomtestcard2

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

randomtestresults: randomtestcard1.out randomtestcard2.out randomtestadventurer.out 

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
