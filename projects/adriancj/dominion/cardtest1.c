/***********************************************************
 * Joel Adriance
 * 2-3-2018
 * CS362 - Winter 2018
 * Assignment 3
 *
 * Unit test for smithy action
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dominion.h"
#include "testUtils.h"

/* After playing Smithy:
 * - The last card discarded should be a Smithy
 * - Three cards should move from the players deck to their hand
 * - The rest of the deck and hand should be unchanged
 * - Other player's hands should be unchanged
 * - Supply piles should be unchanged
 */

int main (int argc, char** argv) {
	
	int success = 1;
	
	printf("Running cardtest1 (smithy)...\n");
	struct gameState* before = malloc(sizeof(struct gameState));
	struct gameState* after = malloc(sizeof(struct gameState));
	
	memset(before, 0, sizeof(struct gameState));
	int* kingdomCards = GetRandomizedKingdomCards(1);
	if (initializeGame(2, kingdomCards, 1, before) != -1) {
		// Put a smithy card in the player's hand at position 0
		before->hand[0][0] = smithy;
		memcpy(after, before, sizeof(struct gameState));
		// Test its effect
		playCard(0, 0, 0, 0, after);
		printf("  -Action Player-\n");
		success &= TestVal(after->handCount[0], before->handCount[0] + 2, "    Cards in hand");
		success &= TestVal(after->deckCount[0], before->deckCount[0] - 3, "    Cards in deck");
		success &= TestVal(after->discardCount[0], before->discardCount[0] + 1, "    Discarded cards");
		// The last discard should be a smithy card
		int discards = after->discardCount[0];
		success &= Test(after->discard[0][discards-1] == smithy, "    Discarded a smithy");
		success &= Test(HasCard(smithy, after->playedCards, after->playedCardCount), "     Smithy in played cards");
		printf("  -Other player-\n");
		success &= Test(DecksMatch(after, before, 1), "    Deck unchanged");
		success &= Test(HandsMatch(after, before, 1), "    Hand unchanged");
		success &= Test(after->discardCount[1] == before->discardCount[1], "    Discard unchanged");
		
		success &= Test(AllSuppliesMatch(after, before), "  Supplies unchanged");
	}
	else {
		printf("  Failed to initialize!\n");
		success = 0;
	}
	if (success) 
		printf("Unit tests PASSED\n");
	else
		printf("Unit tests FAILED\n");
	free(before);
	free(after);
	free(kingdomCards);
	return 0;
}