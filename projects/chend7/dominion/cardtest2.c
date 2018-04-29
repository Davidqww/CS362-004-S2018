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

// Testing card: Smithy
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

	printf("\n------------------------------ TESTING CARD : Smithy ------------------------------\n\n");
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("Player1 has two more cards in hand (+3 draw -1 smithy) :: Hand Count\n");
	failCount += isEqual(G.handCount[player1] + 2, testG.handCount[player1]);	// Intentional bug (draw 2 cards)

	printf("Player1 deck pile has three less cards :: Deck count\n");
	failCount += isEqual(G.deckCount[player1] - 3, testG.deckCount[player1]);

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