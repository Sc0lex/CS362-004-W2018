/***********************************************************
 * Joel Adriance
 * 2-17-2018
 * CS362 - Winter 2018
 * Assignment 4
 *
 * Random test for Smithy card
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dominion.h"
#include "testUtils.h"

/* After playing Smithy:
 * - The last card discarded should be a Smithy
 * - Three cards should move from the players deck to their hand
 * - The rest of the deck and hand should be unchanged
 * - Other player's hands should be unchanged
 * - Supply piles should be unchanged
 */
#define NUM_TESTS 20000
 
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
			// Put a smithy card in the player's hand at position 0
			before->hand[0][0] = smithy;
			memcpy(after, before, sizeof(struct gameState));
			// Test its effect
			playCard(0, 0, 0, 0, after);

			TestAndReport(pReport, after->handCount[0] == before->handCount[0] + 2, "Player did not add two cards to hand", randomSeed);
			TestAndReport(pReport, after->deckCount[0] == before->deckCount[0] - 3, "Player's deck did not have three less cards", randomSeed);
			TestAndReport(pReport, after->discardCount[0] == before->discardCount[0] + 1, "Player didn't discard gained a card", randomSeed);
			// The last discard should be a smithy card
			int discards = after->discardCount[0];
			TestAndReport(pReport, after->discard[0][discards-1] == smithy, "Player didn't discard a smithy", randomSeed);
			TestAndReport(pReport, HasCard(smithy, after->playedCards, after->playedCardCount), "Smithy was not in played cards", randomSeed);

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