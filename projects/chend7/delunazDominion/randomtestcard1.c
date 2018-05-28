#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"
#include "dominion.h"
#include "dominion_helpers.h"

const int TEST_COUNTER = 500;	//number of iterations for test
const int CARDS_TO_DRAW = 3;

int cardEffectFailCount = 0;
int drawCardFailCount = 0;
int handFailCount = 0;
int deckFailCount = 0;
int discardFailCount = 0;
int discardCardFailCount = 0;

int checkSmithyCard(int p, struct gameState *post) {
	int testFailed = 0;

	int cardDrawn = 0;
	int r = 0;
	int bonus = 0;

	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	r = cardEffect(smithy, 0, 0, 0, post, 0, &bonus);
	// Successfully running smithy cardEffect returns 0
	if (r != 0) {
		cardEffectFailCount++;
		testFailed = 1;
	}

	while (cardDrawn < CARDS_TO_DRAW) {
		if (drawCard(p, &pre) == 0) {
			cardDrawn++;
		}
		else {
			drawCardFailCount++;
			testFailed = 1;
		}
	}

	if (discardCard(0, p, &pre, 0) != 0) {
		discardCardFailCount++;
		testFailed = 1;
	}

    if (pre.handCount[p] != post->handCount[p]) {
    	handFailCount++;
    	testFailed = 1;
    }

    if (pre.deckCount[p] != post->deckCount[p]) {
    	deckFailCount++;
    	testFailed = 1;
    }

    if (pre.discardCount[p] != post->discardCount[p]) {
    	discardFailCount++;
    	testFailed = 1;
    }
    return testFailed;
}


int main() {

	printf("\n------------------------------ Randomtest : Smithy ------------------------------\n\n");
	printf("NUMBER OF ITERATION(S): %d\n", TEST_COUNTER);

	int failCount = 0;
	int p = 0;
	struct gameState G;

	int i, j;
	for (i = 0; i < TEST_COUNTER; i++) {
		for (j = 0; j < sizeof(struct gameState); j++) {
			((char*)&G)[j] = floor(Random() * 256);
		}
		p = floor(Random() * 2);
		G.whoseTurn = p;
		G.playedCardCount = 0;
		G.discardCount[p] = floor(Random() * MAX_DECK) - 1;
		G.handCount[p] = floor(Random() * MAX_HAND) - CARDS_TO_DRAW;
		G.deckCount[p] = floor(Random() * MAX_DECK);

		if (checkSmithyCard(p, &G)) {
			failCount++;
		}
	}


	if (failCount == 0) {
		printf("ALL RANDOM TEST ITERATIONS PASSED\n");
	}
	else {
		printf("NUMBER OF FAILED ITERATIONS: %d\n", failCount);
		printf("cardEffectFailCount: %d\n", cardEffectFailCount);
		printf("drawCardFailCount: %d\n", drawCardFailCount);
		printf("handFailCount: %d\n", handFailCount);
		printf("deckFailCount: %d\n", deckFailCount);
		printf("discardFailCount: %d\n", discardFailCount);
		printf("discardCardFailCount: %d\n", discardCardFailCount);
	}

	return 0;
}
