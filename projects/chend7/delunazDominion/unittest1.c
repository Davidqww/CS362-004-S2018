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

// Testing function: isGameOver() 
int main() {
	int failCount = 0;

	int seed = 1000;
	int numberOfPlayers = 2;
	struct gameState G;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};

	initializeGame(numberOfPlayers, k, seed, &G);

	printf("\n------------------------------ TESTING FUNCTION : isGameOver() ------------------------------\n\n");

	//if stack of Province cards is empty, the game ends
	printf("Empty out Province card - game ends\n");
	G.supplyCount[province] = 0;
	failCount += isEqual(1, isGameOver(&G));
	G.supplyCount[province] = 1;

	//if three supply pile are at 0, the game ends
	printf("Empty out three supply pile - game ends\n");
	G.supplyCount[adventurer] = 0;
	G.supplyCount[gardens] = 0;
	G.supplyCount[embargo] = 0;
	failCount += isEqual(1, isGameOver(&G));

	//make is so there are only two empty piles, the game does not end
	printf("Put back a supply pile - game continues\n");
	G.supplyCount[adventurer] = 1;
	failCount += isEqual(0, isGameOver(&G));

	if (failCount) 
		printf("TEST(S) FAILED : %d FAILURES\n", failCount);
	else
		printf("TEST SUCCESSFULL\n");

	return 0;
}