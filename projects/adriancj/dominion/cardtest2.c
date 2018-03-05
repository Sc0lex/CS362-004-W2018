/***********************************************************
 * Joel Adriance
 * 2-3-2018
 * CS362 - Winter 2018
 * Assignment 3
 *
 * Unit test for adventurer action
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dominion.h"
#include "testUtils.h"

/* After playing Adventurer:
 * - There should be two more treasure cards in the players hand
 * - Non-treasure cards drawn should go to discard
 * - The adventurer card should be in discard
 * - Other player hands and supply should not change
 */

int main (int argc, char** argv) {
	
	int success = 1;
	
	printf("Running cardtest2 (adventurer)...\n");
	struct gameState* before = malloc(sizeof(struct gameState));
	struct gameState* after = malloc(sizeof(struct gameState));
	
	memset(before, 0, sizeof(struct gameState));
	int* kingdomCards = GetRandomizedKingdomCards(1);
	if (initializeGame(2, kingdomCards, 1, before) != -1) {
		
		// Set up player zero so they have an adventurer at handpos 0 and
		// so their deck has 1 copper, 2 non-treasure cards, then 1 silver, then 1 gold
		before->hand[0][0] = adventurer;
		before->deck[0][0] = gold;
		before->deck[0][2] = duchy;
		before->deck[0][3] = estate;
		before->deck[0][4] = copper;
		before->deckCount[0] = 4;
		before->handCount[0] = 1;
		
		memcpy(after, before, sizeof(struct gameState));
		// Test its effect
		playCard(0, 0, 0, 0, after);
		
		printf("  -Action Player-\n");
		// Should have two more cards since adventurer should get discarded
		success &= TestVal(after->handCount[0], before->handCount[0] + 1, "    Cards in hand"); // Should be 1 more since adventurer is discarded
		// Deck should be empty
		success &= TestVal(after->deckCount[0], 0, "    Cards in deck");
		// Discards should contain adventurer, the duchy and the estate
		success &= TestVal(after->discardCount[0], 3, "    Discarded cards");
		
		// Check that the correct cards were discarded
		int discardedAction = HasCard(adventurer, after->discard[0], after->discardCount[0]);
		int discardedDuchy  = HasCard(duchy, after->discard[0], after->discardCount[0]);
		int discardedEstate = HasCard(estate, after->discard[0], after->discardCount[0]);
		success &= Test(discardedAction, "    Discarded Adventurer");
		success &= Test(discardedDuchy && discardedEstate, "    Discarded non-treasure cards");
				
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