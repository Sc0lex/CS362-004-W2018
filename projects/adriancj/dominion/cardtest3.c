/***********************************************************
 * Joel Adriance
 * 2-3-2018
 * CS362 - Winter 2018
 * Assignment 3
 *
 * Unit test for great hall action
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dominion.h"
#include "testUtils.h"

/* After playing Great Hall:
 * - The action player should still have one action remaining
 * - The action player should gain one card in their hand and lose one from their deckCount
 * - Other player hands, decks, discards and supply piles should be unchanged
 * - The victory value is not checked here since it is not part of the action effect
 */

int main (int argc, char** argv) {
	
	int success = 1;
	
	printf("Running cardtest3 (great_hall)...\n");
	struct gameState* before = malloc(sizeof(struct gameState));
	struct gameState* after = malloc(sizeof(struct gameState));
	
	memset(before, 0, sizeof(struct gameState));
	int* kingdomCards = GetRandomizedKingdomCards(1);
	if (initializeGame(2, kingdomCards, 1, before) != -1) {
		
		// Set up player zero so they have a great hall at handpos 0 and
		// so their deck has 1 copper and 4 duchies, the copper should be drawn next.
		before->hand[0][0] = great_hall;
		before->deck[0][0] = copper;
		before->deck[0][1] = duchy;
		before->deck[0][2] = duchy;
		before->deck[0][3] = duchy;
		before->deck[0][4] = duchy;
		before->deckCount[0] = 5;
		
		memcpy(after, before, sizeof(struct gameState));
		// Test its effect
		playCard(0, 0, 0, 0, after);
		
		printf("  -Action Player-\n");
		// Should have one more action
		success &= TestVal(after->numActions, 1, "    Gained action");
		// Should have one more card in hand and one less in deck,
		// but great hall is discarded so count should return to 5
		success &= TestVal(after->handCount[0], before->handCount[0], "    Cards in hand");
		success &= TestVal(after->deckCount[0], before->deckCount[0]-1, "    Cards in deck");
		int tookCopper = after->hand[0][after->handCount[0]-1] == copper;
		success &= Test(tookCopper, "    Drew first deck card");
		
		// Discards should contain only the great hall
		success &= TestVal(after->discardCount[0], 1, "    Discarded cards");
		
		// Check that the correct cards were discarded
		int discardedAction = HasCard(great_hall, after->discard[0], after->discardCount[0]);
		success &= Test(discardedAction, "    Discarded Great Hall");
		
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