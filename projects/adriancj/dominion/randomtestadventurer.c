/***********************************************************
 * Joel Adriance
 * 2-17-2018
 * CS362 - Winter 2018
 * Assignment 4
 *
 * Random test for Adventurer card
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dominion.h"
#include "testUtils.h"

/* After playing Adventurer:
 * - There should be two more treasure cards in the players hand
 * - Non-treasure cards drawn should go to discard
 * - The adventurer card should be in discard
 * - Other player hands and supply should not change
 */
 
 #define NUM_TESTS 20000
 
 // Assumes that there is room and that unassigned cards are -1
 void RandomCards(int cards[], int maxCards, int numNew, int minCard, int maxCard) {
	 if (maxCards == 0)
		 return;
	 int i;
	 for (i = 0; i < numNew; ++i) {
		int card;
		int index;
		do {
			card = rand() % (maxCard - minCard + 1) + minCard;
			index = rand() % maxCards;
		} while (cards[index] != -1);
		cards[index] = card;
	 }
 }
 
 // Create a random deck and hand.
 void RandomCardSet(int cards[], int numCards) {
	 if (numCards == 0)
		 return;
	int numCoinCards = rand() % numCards;
	int numVictoryCards, numKingdomCards;
	if (numCards - numCoinCards > 0) {
		numVictoryCards = rand() % (numCards - numCoinCards);
		if (numCards - numCoinCards - numVictoryCards > 0)
			numKingdomCards = rand() % (numCards - numCoinCards - numVictoryCards);
		else
			numKingdomCards = 0;
	}
	 // Now build the set of cards
	 int i;
	 for (i = 0; i < numCards; ++i)
		 cards[i] = -1;

	RandomCards(cards, numCards, numCoinCards,    copper,     gold);
	RandomCards(cards, numCards, numVictoryCards, estate,     province);
	RandomCards(cards, numCards, numKingdomCards, adventurer, treasure_map);
	// Fill the rest with curse cards
	for (i = 0; i < numCards; ++i) {
		if (cards[i] == -1)
			cards[i] = curse;
	}
 }

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
			// Create a special random hand  and deck for the player of different type of cards
			// We limit decks and hands to at least 5 starting cards
			int numHandCards = rand() % (MAX_HAND - 5) + 5;
			int numDeckCards;
			if (i < (int)(i * .01)) // Create the empty deck scenario 1% of the time
				numDeckCards = 0;
			else {
				if (numHandCards < MAX_DECK - 5)
					numDeckCards = rand() % (MAX_DECK - numHandCards - 5) + 5;
				else
					numDeckCards = 5;
			}
			int numDiscards;
			if (MAX_DECK - numDeckCards > 1)
				numDiscards = rand() % (MAX_DECK - numDeckCards);
			else
				numDiscards = 1;
			
			//printf("Num hand cards: %d\n", numHandCards);
			//printf("Num deck cards: %d\n", numDeckCards);
			RandomCardSet(before->hand[0], numHandCards);
			RandomCardSet(before->deck[0], numDeckCards);
			RandomCardSet(before->discard[0], numDiscards);
			before->handCount[0] = numHandCards;
			before->deckCount[0] = numDeckCards;
			before->discardCount[0] = numDiscards;
			before->hand[0][0] = adventurer;
			
			memcpy(after, before, sizeof(struct gameState));
			// Test its effect
			playCard(0, 0, 0, 0, after);
			int deckTreasures = CountCards(copper, before->deck[0], before->deckCount[0]) + 
								CountCards(silver, before->deck[0], before->deckCount[0]) + 
								CountCards(gold,   before->deck[0], before->deckCount[0]);
			if (deckTreasures < 1) { // No treasure cards to draw. Do nothing? OR Run out
				TestAndReport(pReport, after->handCount[0] == before->handCount[0], "Player had no treasure cards in deck, but hand changed", randomSeed);
				TestAndReport(pReport, after->deckCount[0] == before->deckCount[0] || // Allow for no change or all discarded
									   after->deckCount[0] == 0, "Player had no treasure cards in deck, but deck changed", randomSeed);
			} else if (deckTreasures < 2) { // Draw the only treasure card
			    // Less than two treasure cards in deck
				TestAndReport(pReport, after->handCount[0] == before->handCount[0] + 1, "Player had 1 treasure card, but didn't add it to hand", randomSeed);
				// Determine how many should have been drawn to find it
				int cnt = 0; 
				while ((before->deck[0][cnt] != copper) && (before->deck[0][cnt] != silver) && (before->deck[0][cnt] != gold))
					cnt++;
				TestAndReport(pReport, after->deckCount[0] == before->deckCount[0] - cnt - 1, "Player drew the wrong number of cards", randomSeed);
				// cnt is the number of non-treasure cards drawn, but adventurer must also be discarded
				TestAndReport(pReport, after->discardCount[0] == before->discardCount[0] + cnt + 1, "Player didn't discard the right number of cards", randomSeed);
			} else { // Able to draw two treasure cards
				TestAndReport(pReport, after->handCount[0] == before->handCount[0] + 2, "Player had 2 treasure card, but didn't add them to hand", randomSeed);
				int cnt = 0;
				int numFound = 0;
				while (numFound < 2) {
					while ((before->deck[0][cnt] != copper) && (before->deck[0][cnt] != silver) && (before->deck[0][cnt] != gold))
						cnt++;
					cnt++; // for the treasure card
					numFound++;
				}
				TestAndReport(pReport, after->deckCount[0] == before->deckCount[0] - cnt - 1, "Player drew the wrong number of cards", randomSeed);
				// NOTE: cnt includes 1st drawn treasure card, but not second, but the adventurer card itself should also be discarded
				TestAndReport(pReport, after->discardCount[0] == before->discardCount[0] + cnt, "Player didn't discard the right number of cards", randomSeed);
			}
			TestAndReport(pReport, HasCard(adventurer, after->playedCards, after->playedCardCount), "Adventurer was not in played cards", randomSeed);

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