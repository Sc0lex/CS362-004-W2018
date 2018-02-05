/***********************************************************
 * Joel Adriance
 * 2-3-2018
 * CS362 - Winter 2018
 * Assignment 3
 *
 * Unit test for fullDeckCount
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dominion.h"
#include "testUtils.h"

#define NUM_ITERATIONS 10000


/*
 * fullDeckCount is intended to total all cards in a players possession, including
 * those in their deck, hand and discard pile. I used a series of randomly generated hands/discards/decks
 * too check that the totals are correct.
 */
 
int main (int argc, char** argv) {
	printf("Running unittest4 (fullDeckCount)...\n");
	int success = 1;
	int numTypes = treasure_map + 1;
	srand(time(NULL));
	
	// Create a two player game state, create fake hands/discards/decks and verify totals
	struct gameState* state = malloc(sizeof(struct gameState));
	int* kingdomCards = GetRandomizedKingdomCards(1);
	
	if (initializeGame(2, kingdomCards, 1, state) != -1) {
		int i;
		for (i = 0; i < NUM_ITERATIONS; ++i) {
			// For each iteration, create a set of random cards sets split between hand
			// deck and discard
			state->deckCount[0] = rand() % MAX_DECK;
			state->handCount[0] = rand() % MAX_DECK;
			state->discardCount[0] = rand() % MAX_DECK;
			int j;
			for (j = 0; j < state->deckCount[0]; ++j)
				state->deck[0][j] = rand() % numTypes;
			for (j = 0; j < state->handCount[0]; ++j)
				state->hand[0][j] = rand() % numTypes;
			for (j = 0; j < state->discardCount[0]; ++j)
				state->discard[0][j] = rand() % numTypes;
			
			// Check the total for each type
			int type;
			for (type = 0; type < numTypes; type++) {
				int numDeck    = CountCards(type, state->deck[0],    state->deckCount[0]);
				int numHand    = CountCards(type, state->hand[0],    state->handCount[0]);
				int numDiscard = CountCards(type, state->discard[0], state->discardCount[0]);
				
				success &= (numDeck + numHand + numDiscard == fullDeckCount(0, type, state));
			}
		}
	}
	else {
		printf("  Initialization failed!\n");
		success = 0;
	}
	if (success)
		printf("Unit test 4 PASSED\n");
	else
		printf("Unit test 4 FAILED\n");
	return 0;
}