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

#define NUM_ITERATIONS = 1000

gameState* GetGame(int numPlayers, 

/*
 * fullDeckCount is intended to total all cards in a players possession, including
 * those in their deck, hand and discard pile.
 */
 
int main (int argc, char** argv) {
	printf("Running unittest4 (fullDeckCount)...\n");
	int success = 1;
	srand(unsigned int (time(NUL)));
	
	// Create a two player game state, create fake hands/discards/decks and verify totals
	struct gameState* state = malloc(sizeof(gameState));
	int* kingdomCards = GetRandomizedKingdomCards(1);
	
	if (initializeGame(2, kingdomCards, 1, state) != -1) {
		int i; 
		for (i = 0; i < NUM_ITERATIONS; ++i) {
			int card = rand() % (treasure_map - curse + 1);
			state->deckCount[player] = rand() % (MAX_DECK + 1);
			state->handCount[player] = rand() % (MAX_HAND + 1);
			state->discardCount[player] = rand() % (MAX_DECK + MAX_HAND + 1);
			
			TestVal2(fullDeckCount(player, 
			
			int fullDeckCount(int player, int card, struct gameState *state)
		}
	}
	else {
		printf("  Initialization failed!\n");
		success = 0;
	}
	
	return 0;
}