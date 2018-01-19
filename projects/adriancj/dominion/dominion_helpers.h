#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state, 
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);

// Specific card effect functions		   
int smithyEffect(struct gameState *state, int currentPlayer, int handPos);
int adventurerEffect(struct gameState *state, int currentPlayer);
int feastEffect(struct gameState *state, int currentPlayer, int choice);
int stewardEffect(struct gameState *state, int currentPlayer, int handPos, 
				     int optionChoice, 
					 int trashChoice1, 
					 int trashChoice2);
int baronEffect(struct gameState *state, int currentPlayer, int optionChoice);

#endif
