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
 
 #include "dominion.h"
 #include "interface.h" // For card count macros
 #include "testUtils.h"
 
 #define PASS_MSG "PASSED"
 #define FAIL_MSG "FAILED"
 

FailuresReport* NewReport() {
	FailuresReport* pReport = malloc(sizeof(FailuresReport));
	memset(pReport, 0, sizeof(FailuresReport));
	pReport->numTypes = 0;
	return pReport;
}

// Return -1 if not found, otherwise return the index of the report type
int GetReportIndex(FailuresReport* pReport, char* description)
{
	int i;
	for (i = 0; i < pReport->numTypes; ++i){
		if (strcmp(description, pReport->typeDescriptions[i]) == 0)
			return i;
	}
	return -1;
}

void TestAndReport(FailuresReport* pReport, int condition, char* description, unsigned int testSeed){
	if (!condition)
		ReportFailure(pReport, description, testSeed);
}

// Add the report type if it isn't already there. Add thee sample if we haven't already collected enough
void ReportFailure(FailuresReport* pReport, char* description, unsigned int testSeed) {
	int iReport = GetReportIndex(pReport, description);
	if (iReport < 0) {
		if (pReport->numTypes < MAX_TEST_TYPES) {
			iReport = pReport->numTypes++;
			pReport->typeDescriptions[iReport] = malloc(sizeof(char) * strlen(description) + 1);
			strcpy(pReport->typeDescriptions[iReport], description);
			pReport->typeTotalCounts[iReport] = 0;
			pReport->typeSampleCounts[iReport] = 0;
		} else {
			return; // Not allowed to add too many
		}
	}
	pReport->typeTotalCounts[iReport]++;
	// Add the sample failure unless we have reached the max
	if (pReport->typeSampleCounts[iReport] < MAX_TYPE_SAMPLES) {
		pReport->sampleSeeds[iReport][pReport->typeSampleCounts[iReport]++] = testSeed;
	}
}

int FailureTypeCount(FailuresReport* pReport) {
	return pReport->numTypes;
}
long TotalFailureCount(FailuresReport* pReport) {
	long total = 0;
	int i;
	for (i = 0; i < pReport->numTypes; ++i)
		total += pReport->typeTotalCounts[i];
	return total;
}

void PrintFailures(FailuresReport* pReport) {
	int iType, iSample;
	printf("Summary of %d test type failures:\n", pReport->numTypes);
	for (iType = 0; iType < pReport->numTypes; ++iType) {
		printf("  %s\n", pReport->typeDescriptions[iType]);
		printf("    Total failures: %ld\n", pReport->typeTotalCounts[iType]);
		printf("    First %d failure seeds: \n", pReport->typeSampleCounts[iType]);
		for (iSample = 0; iSample < pReport->typeSampleCounts[iType]; ++iSample) {
			printf("      %d\n", pReport->sampleSeeds[iType][iSample]);
		}
	}
}

void DeleteReport(FailuresReport* pReport) {
	int i;
	for (i = 0; i < pReport->numTypes; ++i) 
		free(pReport->typeDescriptions[i]);
	free(pReport);
}


 // Returns 1 if true, 0 if false
int HasDuplicates(int pCards[], int numCards) {
	int i, j;
	for (i = 0; i < numCards; ++i) {
		for (j = 0; j < numCards; ++j) {
			if (i != j && pCards[i] == pCards[j])
				return 1;
		}
	}
	return 0;
}

// Returns 1 if true, 0 if false
int HasCard(int card, int pCards[], int numCards) {
	int i;
	for (i = 0; i < numCards; ++i) {
		if (pCards[i] == card)
			return 1;
	}
	return 0;
}

// Used for testing initialization base on kingdom cards
// This DOES NOT ensure that their are no duplicates unless the flag is set!
// The uniqueness can be checked using HasDuplicates
int* GetRandomizedKingdomCards(int forceUnique) {
	int cnt = 0;
	int* kingdomCards = malloc(sizeof(int) * NUM_K_CARDS);
	memset(kingdomCards, 0, sizeof(int) * NUM_K_CARDS);
	
	while (cnt < NUM_K_CARDS) {
		do {
			kingdomCards[cnt] = (rand() % (treasure_map - adventurer + 1) + adventurer);
		} while (HasDuplicates(kingdomCards, cnt+1) && forceUnique);
		cnt++;
	}
	return kingdomCards;
}
 
// Same as TestVal but doesn't provide an argument for a test value
int Test(int condition, const char* pTestDescription) {
	if (condition) {
		printf("%s - %s\n", pTestDescription, PASS_MSG);
	}
	else {
		printf("%s - %s\n", pTestDescription, FAIL_MSG);
	}
	return condition;
}

// Returns 1 if the testVal is equal to the expectedVal. Otherwise returns 0;
int TestVal(int testVal, int expectedVal, const char* testDescription) {
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

// Returns 1 if the player has the same hand in both game states. Returns 0 otherwise.
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

// Returns 1 if the supply counts for the given card are equal in both game states. Returns 0 otherwise
int SuppliesMatch(struct gameState* state1, struct gameState* state2, int card) {
	return (state1->supplyCount[card] == state2->supplyCount[card]);
}

// Returns 1 if the counts in ALL supply piles match in both game states. Otherwise, returns 0
int AllSuppliesMatch(struct gameState* state1, struct gameState* state2){
	int i;
	for (i = 0; i <= treasure_map; ++i) {
		if (!SuppliesMatch(state1, state2, i))
			return 0;
	}
	return 1;
}

// Check matching cards without considering order. Return 1 if same, 0 if not
int SameCardCombo(int* set1, int count1, int* set2, int count2) {
	if (count1 != count2)
		return 0;
	int numTypes = (int)treasure_map + 1;
	
	// Count the cards of each type and compare
	int* cardCnts1 = malloc(sizeof(int) * numTypes);
	int* cardCnts2 = malloc(sizeof(int) * numTypes);
	memset(cardCnts1, 0, sizeof(int) * numTypes);
	memset(cardCnts2, 0, sizeof(int) * numTypes);
	int i;
	// Count the number of each type
	for (i = 0; i < count1; ++i) {
		cardCnts1[set1[i]] += 1;
		cardCnts2[set2[i]] += 1;
	}
	// Compare the counts
	for (i = 0; i < numTypes; ++i) {
		if (cardCnts1[i] != cardCnts2[i]) {
			free(cardCnts1);
			free(cardCnts2);
			return 0;
		}
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

// Return the number of cards of the specified type in pCards
int CountCards(int cardType, int pCards[], int numCards) {
	int count =0;
	int i;
	for (i = 0; i < numCards; ++i) {
		if (pCards[i] == cardType)
			count++;
	}
	return count;
}

/* Return 1 if the 
int CheckSupply(struct gameState* pState, int card, int count) {
	assert(card >= 0 && card <= treasure_map);
	if (pState->supplyCount[card] == count)
		return 1;
	else
		return 0;
}

int CheckHand(struct gameState* pState, int player, int card, int count) {
	assert(card >= 0 && card <= treasure_map);
	assert(player >= 0 && player < pState->numPlayers);
	
	if (pState->hand[player][card] == count)
		return 1;
	else
		return 0;
}

int CheckDiscard(struct gameState* pState, int player, int card, int count) {
	assert(card >= 0 && card <= treasure_map);
	assert(player >= 0 && player < pState->numPlayers);
	
	if (pState->discard[player][card] == count)
		return 1;
	else
		return 0;
}

int CheckDeck(struct gameState* pState, int player, int card, int count) {
	assert(card >= 0 && card <= treasure_map);
	assert(player >= 0 && player < pState->numPlayers);
	
	if (pState->deck[player][card] == count)
		return 1;
	else
		return 0;
}*/
