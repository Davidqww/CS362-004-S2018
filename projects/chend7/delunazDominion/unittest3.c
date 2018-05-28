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

// Testing function: scoreFor() 
int main() {
	int failCount = 0;

	int seed = 1000;
	int player1 = 0;
	int player2 = 1;
	int numberOfPlayers = 2;
	struct gameState G, testG;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};

	initializeGame(numberOfPlayers, k, seed, &G);

	printf("\n------------------------------ TESTING FUNCTION : scoreFor() ------------------------------\n\n");

	memcpy(&testG, &G, sizeof(struct gameState));

	printf("Player1 starting score should be 3\n");
	failCount += isEqual(3, scoreFor(player1, &G));

	printf("Gain an estate card in discard pile, score increased by 1\n");
	gainCard(estate, &G, 0, player1);
	failCount += isEqual(4, scoreFor(player1, &G) + 1);

	printf("Gain a duchy card in deck, score should be 7\n");
	gainCard(duchy, &G, 1, player1);
	failCount += isEqual(scoreFor(player1, &G), 7);

	printf("Gain a province card in hand, score should be 13\n");
	gainCard(province, &G, 2, player1);
	failCount += isEqual(13, scoreFor(player1, &G));

	printf("Gain a curse card in hand, score should be 12\n");
	gainCard(curse, &G, 2, player1);	
	failCount += isEqual(12, scoreFor(player1, &G));

	printf("Player2 starting score should be 3\n");
	failCount += isEqual(3, scoreFor(player2, &testG));	

	printf("Player2 victory points is unchanged :: Victory Point Count\n");
	failCount += isEqual(scoreFor(player2, &G), scoreFor(player2, &testG));

	if (failCount) 
		printf("\nTEST(S) FAILED : %d FAILURES\n", failCount);
	else
		printf("\nTEST SUCCESSFULL\n");

	return 0;
}