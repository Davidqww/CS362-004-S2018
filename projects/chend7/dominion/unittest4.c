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

// Testing function: supplyCount() 
int main() {
	int failCount = 0;

	int seed = 1000;
	int player1 = 0;
	int player2 = 1;
	int numberOfPlayers = 2;
	struct gameState G;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};

	initializeGame(numberOfPlayers, k, seed, &G);

	printf("\n------------------------------ TESTING FUNCTION : supplyCount() ------------------------------\n\n");

	printf("Check supply count for a card not in any of the 10 supply pile\n");
	failCount += isEqual(supplyCount(great_hall, &G), -1);

	printf("Supply count of smithy should start off at 10\n");
	failCount += isEqual(10, supplyCount(smithy, &G));

	printf("Player1 gains smithy card from supply pile. Supply count of smithy decreases by 1\n");
	int smithyNumber = G.supplyCount[smithy];
	gainCard(smithy, &G, 0, player1);
	failCount += isEqual(smithyNumber - 1, supplyCount(smithy, &G));

	printf("Player2 gains smithy card from supply pile. Supply count of smithy decreases by 1\n");
	smithyNumber = G.supplyCount[smithy];
	gainCard(smithy, &G, 0, player2);
	failCount += isEqual(smithyNumber - 1, supplyCount(smithy, &G));

	if (failCount) 
		printf("TEST(S) FAILED : %d FAILURES\n", failCount);
	else
		printf("TEST SUCCESSFULL\n");

	return 0;
}