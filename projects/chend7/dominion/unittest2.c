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

// Testing function: gainCard() 
int main() {
	int failCount = 0;

	int seed = 1000;
	int player = 0;
	int numberOfPlayers = 2;
	struct gameState G;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};

	initializeGame(numberOfPlayers, k, seed, &G);

	printf("\n------------------------------ TESTING FUNCTION : gainCard() ------------------------------\n\n");

	//check if supply pile is empty (0) or card is not used in game (-1)
	printf("Empty out village pile and try to gain village card\n");
	G.supplyCount[village] = 0;
	failCount += isEqual(gainCard(village, &G, 2, 0), -1);

	// toFlag = 0 : add to discard
	printf("Add smithy card from supply pile to discard pile\n");
	int discardNumber = G.discardCount[player];
	int smithyNumber = G.supplyCount[smithy];
	gainCard(smithy, &G, 0, player);
	failCount += isEqual(discardNumber + 1, G.discardCount[player]);
	printf("Should have one less card in smithy pile\n");
	failCount += isEqual(smithyNumber - 1, G.supplyCount[smithy]);

	// toFlag = 1 : add to deck
	printf("Add smithy card from supply pile to deck\n");
	int deckNumber = G.deckCount[player];
	smithyNumber = G.supplyCount[smithy];
	gainCard(smithy, &G, 1, player);
	failCount += isEqual(deckNumber + 1, G.deckCount[player]);
	printf("Should have one less card in smithy pile\n");
	failCount += isEqual(smithyNumber - 1, G.supplyCount[smithy]);

	// toFlag = 2 : add to hand
	printf("Add smithy card from supply pile to deck\n");
	int handNumber = G.handCount[player];
	smithyNumber = G.supplyCount[smithy];
	gainCard(smithy, &G, 2, player);
	failCount += isEqual(handNumber + 1, G.handCount[player]);
	printf("Should have one less card in smithy pile\n");
	failCount += isEqual(smithyNumber - 1, G.supplyCount[smithy]);

	if (failCount) 
		printf("TEST(S) FAILED : %d FAILURES\n", failCount);
	else
		printf("TEST SUCCESSFULL\n");

	return 0;
}