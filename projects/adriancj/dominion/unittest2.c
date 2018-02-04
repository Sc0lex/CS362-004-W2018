/***********************************************************
 * Joel Adriance
 * 2-3-2018
 * CS362 - Winter 2018
 * Assignment 3
 *
 * Unit test for shuffle
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dominion.h"
#include "testUtils.h"


#define RESHUFFLE_TRIES 10 // How many times we could reshuffle and be reasonably certain
						   // that a different arrangement should have occurred

/* Assumptions
 * - The game state should not change other than for the player whose cards are being shuffled
 * - Cards in the players deck should be rearranged
 * - The cards in the player's hand and deck should not change
 */
 
// Returns 1 if the shuffle was not the same every time, otherwise 0
int ReshuffleCheck(struct gameState* startState, int player) {
	int allSame = 1;
	int i = 0;
	
	struct gameState* firstShuffle = malloc(sizeof(struct gameState));
	struct gameState* thisShuffle = malloc(sizeof(struct gameState));
	
	memcpy(firstShuffle, startState, sizeof(struct gameState));
	shuffle(player, firstShuffle);
	
	// Check if the same shuffle happens every time
	do {
		memcpy(thisShuffle, startState, sizeof(struct gameState));
	
		shuffle(player, thisShuffle);
		
		allSame &= DecksMatch(firstShuffle, thisShuffle, player);
		printf("All Same: %d\n", allSame);
		i++;
	} while(allSame && i < RESHUFFLE_TRIES);
	free(firstShuffle);
	free(thisShuffle);
	if (allSame)
		return 0;
	else
		return 1;
}


int main (int argc, char** argv) {
	printf("Running unittest2 (shuffle)...\n");
	int success = 1;

	struct gameState* pStartState = malloc(sizeof(struct gameState));
	memset(pStartState, 0, sizeof(struct gameState));
	
	int* kingdomCards = GetRandomizedKingdomCards(1);
	
	// Set up a game, trying with different numbers of players
	int numPlayers, player;
	for (numPlayers = 2; numPlayers <= MAX_PLAYERS; numPlayers++)
	{
		printf("  Testing w/ %d-player game...", numPlayers);
		if (initializeGame(numPlayers, kingdomCards, 1, pStartState) == 0) {

			// Shuffle each player's deck, verifying that the state is correct each time
			for (player = 0; player < numPlayers; player++) {
				printf("    Shuffling player %d\n", player);
				// Copy the starting state so we have an uncorrupted state with each shuffle attempt
				struct gameState* pState = malloc(sizeof(struct gameState));
				memcpy(pState, pStartState, sizeof(struct gameState));
				
				shuffle(player, pState);
				
				// Check the player hands and the supplies to ensure that they haven't changed
				int otherPlayer;
				int otherDecksOK = 1;
				int otherHandsOK = 1;
				for (otherPlayer = 0; otherPlayer < numPlayers; ++otherPlayer) {
					if (otherPlayer != player) {
						otherDecksOK &= DecksMatch(pState, pStartState, otherPlayer);
						otherHandsOK &= HandsMatch(pState, pStartState, otherPlayer);
					}
				}
				success &= Test(otherDecksOK, "      Other player decks unchanged");
				success &= Test(otherHandsOK, "      Other player hands unchanged");
				
				// Make sure the player has the same cards in the deck, but that they are rearranged
				
				success &= Test(!DecksMatch(pStartState, pState, player), "      Player deck was changed");
				success &= Test(SameDeckCombo(pStartState, pState, player), "      Player deck has same card combo");
				
				success &= Test(HandsMatch(pStartState, pState, player), "      Player hand unchanged");
				success &= Test(AllSuppliesMatch(pStartState, pState), "      Supply piles unchanged");		
							
				// Reshuffle the player's deck several times and make sure that it isn't reshuffled the same way every time
				success &= Test(ReshuffleCheck(pStartState, player), "      Different shuffles");
				
				free(pState);
			}		
		}
		else {
			printf("    Initialization failed!");
			success = 0;
		}
	}
	if (success)
		printf("Unit tests PASSED\n");
	else
		printf("Unit tests FAILED\n");
	return 0;
}

