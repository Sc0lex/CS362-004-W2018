/***********************************************************
 * Joel Adriance
 * 2-3-2018
 * CS362 - Winter 2018
 * Assignment 3
 *
 * Unit test for buyCard
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dominion.h"
#include "interface.h"
#include "testUtils.h"

/*
 * Assumptions:
 * The player's coins should be reduced by the cost of the card
 * The purchased card should be added to the player's hand
 * Other player hands and decks shouldn't change
 * The supply pile should be reduced only for the purchased card
 * -1 Should be returned if the purchase fails, otherwise return 0
 */
 
 // Provides the ability to modify the game state to test card purchase under different conditions
 int TryBuy(struct gameState* state, int player, int card, int playersTurn, int numBuys, int phase, int coins) {
	int success = 1;
	struct gameState* customState = malloc(sizeof(struct gameState));
	memcpy(customState, state, sizeof(struct gameState));
	
	customState->numBuys = numBuys;
	customState->whoseTurn = playersTurn;
	customState->phase = phase;
	customState->coins = coins;
	
	if (buyCard(card, customState) == -1) {
		free(customState);
		return 0;
	}
	else {
		// Check resulting state
		success &= TestVal2(customState->numBuys, numBuys--, "    Number of buys");
		success &= TestVal2(customState->coins, coins - getCost(card), "    Number of coins");
		if (numBuys-1 > 0 && coins - getCost(card) > 0)
			success &= Test(customState->phase, "    Still in buy phase");
		else
			success &= Test(customState->phase, "    Moved to cleanup phase");
		// The last card in the discard should be the new card
		success &= Test(customState->discard[player][customState->discardCount[player]-1] == card, "    Gained card");
		success &= TestVal2(customState->supplyCount[card], state->supplyCount[card]--, "    Supply reduced");
		// other supplies unchanged
		int i;
		int otherSuppliesOK = 1;
		int numTypes = treasure_map + 1;
		for (i = 0; i < numTypes; ++i) {
			if (i != card)
				otherSuppliesOK &= (state->supplyCount[i] == customState->supplyCount[i]);
		}
		success &= Test(otherSuppliesOK, "    Non-purchased supplies unchanged");
		free(customState);
		return success;
	}
 }

int main (int argc, char** argv) {
	printf("Running unittest3 (buyCard)...\n");
	
	int success = 1;
	
	struct gameState* state = malloc(sizeof(struct gameState));
	int* kingdomCards = GetRandomizedKingdomCards(1);
	
	if (initializeGame(2, kingdomCards, 1, state) == 0) {
		// Have the first player try buying one card that they can and can't afford.
		// Verify other state
		// Try to buy when it is/is not the player's turn
		// Try when there are/are not enough buys
		// Try when it is/is not the right phase
		// Try when the player does/doesn't have enough coin
		
		int card;
		int numTypes = treasure_map + 1;
		for (card = 0; card < numTypes; ++card) {
			// Buy with enough buys and coins
			TryBuy(state, 0, card, 0, 1, BUY_PHASE, getCost(card));
			// Wrong phase
			TryBuy(state, 0, card, 0, 1, CLEANUP_PHASE, getCost(card));
			TryBuy(state, 0, card, 0, 1, ACTION_PHASE, getCost(card));
			// Extra buys
			TryBuy(state, 0, card, 0, 2, BUY_PHASE, getCost(card));
			// Not enough money
			TryBuy(state, 0, card, 0, 1, BUY_PHASE, getCost(card)-1);
			// Not enough buys
			TryBuy(state, 0, card, 0, 0, BUY_PHASE, getCost(card));
			// Wrong turn
			TryBuy(state, 0, card, 1, 1, BUY_PHASE, getCost(card));
		}
		
		success &= Test(TryBuy(state, 0, copper, 0, 1, BUY_PHASE, getCost(copper)), "Card buy test");
	
	}
	else {
		printf("  Initialization failed!\n");
		success = 0;
	}
	free(state);
	if (success)
		printf("Unit tests PASSED\n");
	else
		printf("Unit tests FAILED\n");
	return 0;
}