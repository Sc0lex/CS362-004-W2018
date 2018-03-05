/***********************************************************
 * Joel Adriance
 * 2-17-2018
 * CS362 - Winter 2018
 * Assignment 4
 *
 * Random test for Steward card
 ***********************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dominion.h"
#include "testUtils.h"

/* After playing Steward:
 * - Option 1 (+2 Cards)
 *   - The player gains 1 card in their hand
 *   Option 2 (+2 Treasure)
 *   - The player gains 2 treasure for this turns buys
 *   Option Default (Trash 2)
 *   - Two cards move from the player hand to the trash
 *   
 *   In any case, other hands and decks should remain unchanged
 */
#define NUM_TESTS 20000
 
 // NOTE: When calling play card, choice1 is the option and choices 2 and 3 are the 
 // cards to trash. 
//  playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state) 
 
 
int main (int argc, char** argv) {
	int i;
	unsigned int randomSeed = time(NULL);
	FailuresReport* pReport = NewReport();
	
	for (i = 0; i < NUM_TESTS; ++i) {
		randomSeed = rand();
		srand(randomSeed);
	
		struct gameState* before = malloc(sizeof(struct gameState));
		struct gameState* after = malloc(sizeof(struct gameState));
		memset(before, 0, sizeof(struct gameState));
		int* kingdomCards = GetRandomizedKingdomCards(1);
		// calculate all of the random values for this iteration.
		int numPlayers = 2 + rand() % (MAX_PLAYERS-2);

		if (initializeGame(numPlayers, kingdomCards, randomSeed, before) != -1) {
			// Put a steward card in the player's hand at position 0
			before->hand[0][0] = steward;
			memcpy(after, before, sizeof(struct gameState));
			// Test its effect
			int option = rand() % 3; // (0-2)
			int card1 = 0; // trash cards if applicable
			int card2 = 0; 
			if (option == 0) {
				card1 = rand() % before->handCount[0];
				do {
					card2 = rand() % before->handCount[0];
				} while (card1 != card2);
			}
			playCard(0, option, card1, card2, after);
			if (option == 1) { // Draw 2
				TestAndReport(pReport, after->handCount[0] == before->handCount[0] + 2, "Player chose draw, but did not add two cards to hand", randomSeed);
				TestAndReport(pReport, after->deckCount[0] == before->deckCount[0] - 2, "Player chose draw, but deck did not have two less cards", randomSeed);
			} else if (option == 2) { // Gain 2 coin
				TestAndReport(pReport, after->coins == before->coins + 2, "Player chose coins, but did not gain to coins", randomSeed);
			} else { // Trash two
				TestAndReport(pReport, after->handCount[0] == before->handCount[0] - 2, "Player chose trash, but did not lose two cards from hand", randomSeed);
				TestAndReport(pReport, after->deckCount[0] == before->deckCount[0], "Player chose trash, but their deck changed", randomSeed);
				TestAndReport(pReport, after->discardCount[0] == before->discardCount[0], "Player chose trash, but their discards changed", randomSeed);
			}
			TestAndReport(pReport, after->discardCount[0] == before->discardCount[0] + 1, "Player didn't discard gained a card", randomSeed);
			// The last discard should be a steward card
			int discards = after->discardCount[0];
			TestAndReport(pReport, after->discard[0][discards-1] == steward, "Player didn't discard a steward", randomSeed);
			TestAndReport(pReport, HasCard(steward, after->playedCards, after->playedCardCount), "Steward was not in played cards", randomSeed);

			// Check other hands
			int iOther;
			for (iOther = 1; iOther < numPlayers; ++iOther) {
				TestAndReport(pReport, DecksMatch(after, before, iOther), "Other player's deck changed", randomSeed);
				TestAndReport(pReport, HandsMatch(after, before, iOther), "Other player's hand changed", randomSeed);
				TestAndReport(pReport, after->discardCount[iOther] == before->discardCount[iOther], "Other player's discard changed", randomSeed);
			}
			TestAndReport(pReport, AllSuppliesMatch(after, before), "Supplies changed", randomSeed);
		} else {
			ReportFailure(pReport, "Game failed to initialize", randomSeed);
		}
		free(before);
		free(after);
		free(kingdomCards);
	}
	printf("%d random tests executed\n", NUM_TESTS);
	if (TotalFailureCount(pReport) < 1) 
		printf("Random tests PASSED\n");
	else
		PrintFailures(pReport);
	DeleteReport(pReport);
	return 0;
}