/***********************************************************
 * Joel Adriance
 * 2-3-2018
 * CS362 - Winter 2018
 * Assignment 3
 *
 * Implementations for simple functions related to performing
 * unit tests.
 ***********************************************************/
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 
 #include "interface.h" // For card count macros
 #include "testUtils.h"
 
 #define PASS_MSG "PASSED"
 #define FAIL_MSG "FAILED"
 
 // Returns 1 if true, 0 if false
int HasDuplicates(int pCards[], int numCards)
{
	int i, j;
	for (i = 0; i < numCards; ++i)
	{
		for (j = 0; j < numCards; ++j)
		{
			if (i != j && pCards[i] == pCards[j])
				return 1;
		}
	}
	return 0;
}

// Returns 1 if true, 0 if false
int HasCard(int card, int pCards[], int numCards)
{
	int i;
	for (i = 0; i < numCards; ++i)
	{
		if (pCards[i] == card)
			return 1;
	}
	return 0;
}

// Used for testing initialization base on kingdom cards
// This DOES NOT ensure that their are no duplicates unless the flag is set!
// The uniqueness can be checked using HasDuplicates
int* GetRandomizedKingdomCards(int forceUnique)
{
	int cnt = 0;
	int* kingdomCards = malloc(sizeof(int) * NUM_K_CARDS);
	memset(kingdomCards, 0, sizeof(int) * NUM_K_CARDS);
	
	while (cnt < NUM_K_CARDS)
	{
		do
		{
			kingdomCards[cnt] = (rand() % (treasure_map - adventurer + 1) + adventurer);
		} while (HasDuplicates(kingdomCards, cnt+1) && forceUnique);
		cnt++;
	}
	return kingdomCards;
}

 
 // Returns the result of the test, mirroring the result of 'condition'. 
 // Also prints the test description to the console so bugs can be tracked down.
int TestVal(int condition, const char* pTestDescription, int val) {
	if (condition)
	{
		printf("%s (%d) - %s\n", pTestDescription, val, PASS_MSG);
	}
	else
	{
		printf("%s (%d) - %s\n", pTestDescription, val, FAIL_MSG);
	}
	return condition;
}
 
// Same as TestValm but doesn't provide an argument for a test value
int Test(int condition, const char* pTestDescription) {
	if (condition)
	{
		printf("%s - %s\n", pTestDescription, PASS_MSG);
	}
	else
	{
		printf("%s - %s\n", pTestDescription, FAIL_MSG);
	}
	return condition;
}
 
int TestVal2(int testVal, int expectedVal, const char* testDescription) {
	if (testVal == expectedVal)
		printf("%s - %s\n", testDescription, PASS_MSG);
	else
		printf("%s - %s (Expected: %d Actual: %d)\n", testDescription, FAIL_MSG, expectedVal, testVal);
	return (testVal == expectedVal);
}

// Returns 1 if matching, otherwise 0
int DecksMatch(struct gameState* state1, struct gameState* state2, int player) {
	if (state1->deckCount[player] != state2->deckCount[player])
		return 0;
	int i;
	for (i = 0; i < state1->deckCount[player]; ++i) {
		if (state1->deck[player][i] != state2->deck[player][i])
			return 0;
	}
	return 1;
}

int HandsMatch(struct gameState* state1, struct gameState* state2, int player) {
	if (state1->handCount[player] != state2->handCount[player])
		return 0;
	int i;
	for (i = 0; i < state1->handCount[player]; ++i) {
		if (state1->hand[player][i] != state2->hand[player][i])
			return 0;
	}
	return 1;
}

int SuppliesMatch(struct gameState* state1, struct gameState* state2, int card) {
	//int result;
	return (state1->supplyCount[card] == state2->supplyCount[card]);
	/*	result = 0;
	else
		result = 1;
	if (!result)
		printf("Card id %d supply pile mismatch! %d != %d\n", card, state1->supplyCount[card], state2->supplyCount[card]);
	return result;*/
}

int AllSuppliesMatch(struct gameState* state1, struct gameState* state2){
	int i;
	for (i = 0; i <= treasure_map; ++i) {
		if (!SuppliesMatch(state1, state2, i))
			return 0;
	}
	return 1;
}

// Check matching cards without considering order
int SameCardCombo(int* set1, int count1, int* set2, int count2) {
	if (count1 != count2)
		return 0;
	int numTypes = treasure_map + 1;
	
	// Count the cards of each type and compare
	int* cardCnts1 = malloc(sizeof(int) * numTypes);
	int* cardCnts2 = malloc(sizeof(int) * numTypes);
	memset(cardCnts1, 0, sizeof(int) * numTypes);
	memset(cardCnts2, 0, sizeof(int) * numTypes);
	int i;
	for (i = 0; i < count1; ++i)
		if (set1[i] > -1)
			cardCnts1[i] += set1[i];
		if (set2[i] > -1)
			cardCnts2[i] += set2[i];
	for (i = 0; i < numTypes; ++i) {
		if (cardCnts1[i] != cardCnts2[i])
			free(cardCnts1);
			free(cardCnts2);
			return 0;
	}	
	free(cardCnts1);
	free(cardCnts2);
	return 1;
}

// Check matching cards without considering order
int SameDeckCombo(struct gameState* state1, struct gameState* state2, int player) {
	return SameCardCombo(state1->deck[player], state1->deckCount[player],
						 state2->deck[player], state2->deckCount[player]);
}

// Check matching cards without considering order
int SameHandCombo(struct gameState* state1, struct gameState* state2, int player) {
	return SameCardCombo(state1->hand[player], state1->handCount[player],
						 state2->hand[player], state2->handCount[player]);
}
