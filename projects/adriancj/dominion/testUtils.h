/***********************************************************
 * Joel Adriance
 * 2-3-2018
 * CS362 - Winter 2018
 * Assignment 3
 *
 * Definitions for simple functions related to performing
 * unit tests.
 ***********************************************************/
 #include "dominion.h"
 
 // Returns the result of the test, mirroring the result of 'condition'. 
 // Also prints the test description to the console so bugs can be tracked down.
 int Test(int condition, const char* pTestDescription);
 int TestVal(int testVal, int expectedVal, const char* testDescription);
 
 // Returns 1 if true, 0 if false
int HasDuplicates(int pCards[], int numCards);

// Returns 1 if true, 0 if false
int HasCard(int card, int pCards[], int numCards);

// Return the number of cards of the specified type in pCards
int CountCards(int cardType, int pCards[], int numCards);

// Used for testing initialization base on kingdom cards
// This DOES NOT ensure that their are no duplicates unless the flag is set!
// The uniqueness can be checked using HasDuplicates
int* GetRandomizedKingdomCards(int forceUnique);


// Returns 1 if matching, otherwise 0
int DecksMatch(struct gameState* state1, struct gameState* state2, int player);
int HandsMatch(struct gameState* state1, struct gameState* state2, int player);
int SuppliesMatch(struct gameState* state1, struct gameState* state2, int card);
int AllSuppliesMatch(struct gameState* state1, struct gameState* state2);
// Check matching cards without considering order
int SameCardCombo(int* set1, int count1, int* set2, int count2);
// Check matching cards without considering order
int SameDeckCombo(struct gameState* state1, struct gameState* state2, int player);
// Check matching cards without considering order
int SameHandCombo(struct gameState* state1, struct gameState* state2, int player);
