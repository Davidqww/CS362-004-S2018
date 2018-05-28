#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"
#include "dominion.h"
#include "dominion_helpers.h"

const int TEST_COUNTER = 500;	//number of iterations for test
const int MIN_TREASURE = 4;	//minimum treasure card needed
const int TREASURE_DRAW = 2; 

int cardEffectFailCount = 0;
int drawCardFailCount = 0;
int treasureFailCount = 0;
int handFailCount = 0;
int deckFailCount = 0;
int discardFailCount = 0;

int checkAdventurerCard(int p, struct gameState *post) {
	int testFailed = 0;

	int temphand[MAX_HAND];
	int drawntreasure = 0;
	int cardDrawn = 0;
	int z = 0;
	int r = 0;
	int card = 0;
	int bonus = 0;
	int postTreasureCount = 0;
	int preTreasureCount = 0;

	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));
	r = cardEffect(adventurer, 0, 0, 0, post, 0, &bonus);
	// Successfully running adventurer cardEffect returns 0
	if (r != 0) {
		cardEffectFailCount++;
		testFailed = 1;
	}

	//Bug introduced from assignment-2 gains three treasure cards
	while (drawntreasure < TREASURE_DRAW) {
		if(drawCard(p, &pre) == 0) {
			cardDrawn = pre.hand[p][pre.handCount[p] - 1]; //top card of hand is most recently drawn card.
			if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
				drawntreasure++;
			else{
				temphand[z] = cardDrawn;
				pre.handCount[p]--; //this should just remove the top card (the most recently drawn one).
				z++;
			}
		}
		else {
			drawCardFailCount++;
			testFailed = 1;
		}
	}	
	while(z - 1 >= 0) {
		pre.discard[p][pre.discardCount[p]++] = temphand[z - 1]; // discard all cards in play that have been drawn
		z = z - 1;
	}
	
	// get the treasure counts for both pre and post states
	int i = 0;
    for (i = 0; i < post->handCount[p]; i++) {
        card = post->hand[p][i];
        if (card == copper || card == silver || card == gold) {
            postTreasureCount++;
        }
    }

    for (i = 0; i < pre.handCount[p]; i++) {
        card = pre.hand[p][i];
        if (card == copper || card == silver || card == gold) {
            preTreasureCount++;
        }
    }

    if (preTreasureCount != postTreasureCount) {
    	treasureFailCount++;
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

	printf("\n------------------------------ Randomtest : Adventurer ------------------------------\n\n");
	printf("NUMBER OF ITERATION(S): %d\n", TEST_COUNTER + TEST_COUNTER);

	int failCount = 0;
	int p = 0;
	struct gameState G;
	int treasureType[] = {copper, silver, gold};
	int randTreasure;

	int i, j;
	for (i = 0; i < TEST_COUNTER; i++) {
		for (j = 0; j < sizeof(struct gameState); j++) {
			((char*)&G)[j] = floor(Random() * 256);
		}
		p = floor(Random() * 2);
		G.whoseTurn = p;
		G.discardCount[p] = 0;
		G.handCount[p] = floor(Random() * MAX_HAND);
		G.deckCount[p] = floor(Random() * MAX_DECK);

		while (G.deckCount[p] < MIN_TREASURE)
			G.deckCount[p] = floor(Random() * MAX_DECK);

		//put different types of treasure cards throughout deck
		for (j = G.deckCount[p] - 1; j > 0; j /= 2) {
			randTreasure = Random() * 3;
			G.deck[p][j] = treasureType[randTreasure];
		}

		if (checkAdventurerCard(p, &G)) {
			failCount++;
		}
	}

	for (i = 0; i < TEST_COUNTER; i++) {
		for (j = 0; j < sizeof(struct gameState); j++) {
			((char*)&G)[j] = floor(Random() * 256);
		}
		p = floor(Random() * 2);
		G.whoseTurn = p;
		G.discardCount[p] = floor(Random() * MAX_DECK);
		G.handCount[p] = floor(Random() * MAX_HAND);
		G.deckCount[p] = 0;

		while (G.discardCount[p] < MIN_TREASURE)
			G.discardCount[p] = floor(Random() * MAX_DECK);

		//put different types of treasure cards throughout deck
		for (j = G.discardCount[p] - 1; j > 0; j /= 2) {
			randTreasure = Random() * 3;
			G.discard[p][j] = treasureType[randTreasure];
		}

		if (checkAdventurerCard(p, &G)) {
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
		printf("treasureFailCount: %d\n", treasureFailCount);
		printf("handFailCount: %d\n", handFailCount);
		printf("deckFailCount: %d\n", deckFailCount);
		printf("discardFailCount: %d\n", discardFailCount);
	}

	return 0;
}
