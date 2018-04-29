#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "rngs.h"
#include "dominion.h"
#include "dominion_helpers.h"

int isEqual(int exptectedVal, int actualVal) {
	printf("Expected value: %d\tActual value: %d\n", exptectedVal, actualVal);
	if(exptectedVal == actualVal) {
		printf("::TEST PASSED\n\n");
		return 0;
	}
	else {
		printf("::TEST FAILED\n\n");
		return 1;
	}
}

int treasureCount(int currentPlayer, const struct gameState *state) {
	int treasureCount = 0;

	int i;
	for(i = 0; i < state->handCount[currentPlayer]; i++) {
		if (
			state->hand[currentPlayer][i] == copper ||
			state->hand[currentPlayer][i] == silver ||
			state->hand[currentPlayer][i] == gold 
			)
			treasureCount++;
	}
	return treasureCount;
}

// Testing card: Adventurer
int main() {
	int failCount = 0;

	int seed = 1000;
	int player1 = 0;
	int player2 = 1;
	int numberOfPlayers = 2;
	struct gameState G, testG;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};

	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;

	initializeGame(numberOfPlayers, k, seed, &G);

	printf("\n------------------------------ TESTING CARD : Adventurer ------------------------------\n\n");
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("Print deck order: %d\n", G.deckCount[player1]);
	printf("Smithy\n");
	printf("Smithy\n");
	printf("Copper\n");
	printf("Copper\n");
	printf("Copper\n\n");

	testG.deck[player1][4] = smithy;
	testG.deck[player1][3] = smithy;
	testG.deck[player1][2] = copper;
	testG.deck[player1][1] = copper;
	testG.deck[player1][0] = copper;

	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("Player1 has 2 cards in discard pile (discarded 2 smithy) :: Discard Count\n");
	failCount += isEqual(2, testG.discardCount[player1]);	// Introducced bug where adventurer adds 3 treasures

	printf("Player1 has 1 card in deck pile (discarded 2 smithy, added 2 copper to hand) :: Deck Count\n");
	failCount += isEqual(1, testG.deckCount[player1]);

	printf("Player1 gains two more treasure cards in hand\n");
	failCount += isEqual(treasureCount(0, &G) + 2, treasureCount(0, &testG));

	printf("Player1 has one more card in hand (+2 treasure -1 adventurer) :: Hand Count\n");
	failCount += isEqual(G.handCount[player1] + 1, testG.handCount[player1]); 

	printf("Player1 victory points is unchanged :: Victory Point Count\n");
	failCount += isEqual(scoreFor(player1, &G), scoreFor(player1, &testG));

	printf("Player2 hand is unaffected :: Hand Count\n");
	failCount += isEqual(G.handCount[player2], testG.handCount[player2]);

	printf("Player2 deck is unaffected :: Deck Count\n");
	failCount += isEqual(G.deckCount[player2], testG.deckCount[player2]);

	printf("Player2 victory points is unchanged :: Victory Point Count\n");
	failCount += isEqual(scoreFor(player2, &G), scoreFor(player2, &testG));

	if (failCount) 
		printf("TEST(S) FAILED : %d FAILURES\n", failCount);
	else
		printf("TEST SUCCESSFULL\n");

	return 0;
}