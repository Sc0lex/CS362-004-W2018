/***********************************************************
 * Joel Adriance
 * 2-3-2018
 * CS362 - Winter 2018
 * Assignment 3
 *
 * Unit test for initializeGame()
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dominion.h"
#include "testUtils.h"

#define NUM_KINGDOM 10

// Return 1 if the card has the given embargo count in the game state. Otherwise return 0;
int CheckEmbargos(struct gameState* pState, int card, int count) {
	assert(card >= 0 && card <= treasure_map);
	if (pState->embargoTokens[card] == count)
		return 1;
	else
		return 0;
}

int TestPlayerCounts(struct gameState* state) {
	int success = 1;
	// Test different numbers of players. Only 2,3 and 4 should be allowed.
	printf("  Testing player counts...\n");
	int numPlayers;
	int* kingdomCards = GetRandomizedKingdomCards(1);
	for (numPlayers = -1; numPlayers <= MAX_PLAYERS + 1; numPlayers++) 
	{
		printf("  -%d Players'", numPlayers);
		if (numPlayers < 2 || numPlayers > MAX_PLAYERS)
		{
			success &= Test(initializeGame(numPlayers, kingdomCards, 1, state) == -1, "    Bad player count");
		}
		else
		{
			int goodState = initializeGame(numPlayers, kingdomCards, 1, state) == 0;
			int goodCount = state->numPlayers == numPlayers;
			success &= Test(goodState && goodCount, "    Good player count");
		}
	}
	free(kingdomCards);
	return success;
}

int TestKingdomCards(struct gameState* state) {
	int success = 1;
	// Check that valid kingdom card selections are accepted, but invalid ones are rejected
	printf("  Testing kingdom card initialization...\n");
	int* badKingdomCards = NULL;
	int* goodKingdomCards = GetRandomizedKingdomCards(1);
	do
	{
		if (badKingdomCards != NULL)
			free(badKingdomCards);
		badKingdomCards = GetRandomizedKingdomCards(0);
	} while (!HasDuplicates(badKingdomCards, NUM_KINGDOM));
	
	success &= Test(initializeGame(2, goodKingdomCards, 1, state) == 0,  "    Unique kingdom cards");
	success &= Test(initializeGame(2, badKingdomCards,  1, state) == -1, "    Duplicate kingdom cards");
	
	free(goodKingdomCards);
	free(badKingdomCards);
	return success;
}

/* Assumptions:
 * After initialization the game state should be as follows:
 * - The number of players should set correctly
 * - An error code should be returned if too many players are requested
 * - Each player should have the correct number and type of cards (7 copper, 3 estates)
 * - The piles should be correctly set up for each # of players
 *   - 2 players:  8 of each victory card type, 10 curse cards
 *   - 3 players: 12 of each victory card type, 20 curse cards
 *   - 4 players: 12 of each victory card type, 30 curse cards
 * - There should be 10 Distinct kingdom cards
 * - There should be 60 copper, 40 silver and 30 gold cards total
 */
int main (int argc, char** argv) {
	struct gameState* state = malloc(sizeof(struct gameState));
	int success = 1;
	
	printf("Running unittest1 (initializeGame)...\n");
	
	// Test that invalid player counts fail initialization
	success &= TestPlayerCounts(state);
	
	// Check that invalid kingdom cards are rejected
	success &= TestKingdomCards(state);
	
	// Check the initial card counts for each player and pile according to the player count and selected kingdom cards
	int* KCWithGardens = NULL;
	do {
		if (KCWithGardens != NULL)
			free(KCWithGardens);
		KCWithGardens = GetRandomizedKingdomCards(1);
	} while (!HasCard(gardens, KCWithGardens, NUM_KINGDOM) || HasCard(great_hall, KCWithGardens, NUM_KINGDOM));
		
	int* KCWithGreatHall = NULL;
	do {
		if (KCWithGreatHall != NULL)
			free(KCWithGreatHall);
		KCWithGreatHall = GetRandomizedKingdomCards(1);
	} while(!HasCard(great_hall, KCWithGreatHall, NUM_KINGDOM) || HasCard(gardens, KCWithGreatHall, NUM_KINGDOM));
	
	int* KCWithBoth = NULL;
	do {
		if (KCWithBoth != NULL)
			free(KCWithGreatHall);
		KCWithBoth = GetRandomizedKingdomCards(1);
	} while (!HasCard(gardens, KCWithBoth, NUM_KINGDOM) || !HasCard(great_hall, KCWithBoth, NUM_KINGDOM));
	
	int* KCWithNeither = NULL;
	do {
		if (KCWithNeither != NULL)
			free(KCWithNeither);
		KCWithNeither = GetRandomizedKingdomCards(1);
	} while (HasCard(gardens, KCWithNeither, NUM_KINGDOM) || HasCard(great_hall, KCWithNeither, NUM_KINGDOM));
	
	printf("\tTesting counts...\n");
	int* kingdomSets[4] = {KCWithBoth, KCWithGardens, KCWithGreatHall, KCWithNeither};
	char* setNames[4] = {"Testing w/ Gardens and Great Hall...", 
						 "Testing w/ Gardens and not Great Hall...",
						 "Testing w/ Great Hall and not Gardens...",
						 "Testing w/ neither Great Hall nor Gardens..."};
	int numPlayers;
	for (numPlayers = 2; numPlayers <= MAX_PLAYERS; numPlayers++)
	{	
		printf("    Testing %d-Player Game...\n", numPlayers);
		
		int i;
		for (i = 0; i < 4; ++i)
		{
			int* kingdomCards = kingdomSets[i];
			printf("      %s\n", setNames[i]);
			
			success &= TestVal(initializeGame(numPlayers, kingdomCards, 1, state), 0, "        Initialization");
		
			int numCopper = 60 - 7 * numPlayers;
			int numCurses = 10 * (numPlayers - 1);
			
			success &= TestVal(state->supplyCount[copper], numCopper, "        Copper count");
			success &= TestVal(state->supplyCount[silver], 40,        "        Silver count");
			success &= TestVal(state->supplyCount[gold],   30,        "        Gold count");
			success &= TestVal(state->supplyCount[curse],  numCurses, "        Curse count");
			
			int victoryCnt;
			if (numPlayers == 2)
			{
				victoryCnt = 8;
			}
			else{
				victoryCnt = 12;
			}
			success &= TestVal(state->supplyCount[estate], victoryCnt,   "        Estate count");
			success &= TestVal(state->supplyCount[duchy], victoryCnt,    "        Duchy count");
			success &= TestVal(state->supplyCount[province], victoryCnt, "        Province count");
			
			// Check kingdom cards. 10 of each unless gardens or great_hall, in which case it should match the victory piles
			int card;
			for (card = adventurer; card <= treasure_map; ++card)
			{
				if (HasCard(card, kingdomCards, NUM_KINGDOM))
				{
					if (card == gardens || card == great_hall)
						success &= TestVal(state->supplyCount[card], victoryCnt, "        Victory kingdom card count");
					else
						success &= TestVal(state->supplyCount[card], 10, "        Non-victory kingdom card count");
				}
				else
				{
					success &= TestVal(state->supplyCount[card], -1, "        Unused kingdom card count");
				}
			}
	
			// Check all embargos should be zero
			card = 0;
			int embargoPassed = 1;
			while (card <= treasure_map && embargoPassed)
			{
				if (!CheckEmbargos(state, card, 0))
					embargoPassed = 0;
				card++;
			}
			
			success &= Test(embargoPassed, "        All embargos zeroed"); 
			
			// Check player decks, hands and discards
			int playerNum;
			for (playerNum = 0; playerNum < numPlayers; playerNum++)
			{
				printf("        -Player %d-\n", playerNum);
				int deckCopper = CountCards(copper, state->deck[playerNum], state->deckCount[playerNum]);
				int handCopper = CountCards(copper, state->hand[playerNum], state->handCount[playerNum]);
				int deckEstate = CountCards(estate, state->deck[playerNum], state->deckCount[playerNum]);
				int handEstate = CountCards(estate, state->hand[playerNum], state->handCount[playerNum]);
				
				success &= TestVal(deckCopper + handCopper, 7, "          Coppers");
				success &= TestVal(deckEstate + handEstate, 3, "          Estates");
				success &= TestVal(state->deckCount[playerNum], 5,    "          Deck count");
				success &= TestVal(state->discardCount[playerNum], 0, "          Discards");
				success &= TestVal(state->handCount[playerNum], 5,    "          Hand count");
			}
			success &= TestVal(state->playedCardCount, 0, "        Played cards");
		}
	}
	free(KCWithBoth);
	free(KCWithGardens);
	free(KCWithGreatHall);
	free(KCWithNeither);
	
	
	if (success)
		printf("Unit test 1 PASSED\n");
	else
		printf("Unit test 1 FAILED\n");
	
	
	free(state);
	
	return 0;
}