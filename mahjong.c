#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *library[] = {"d1", "d1", "d1", "d1", "d2", "d2", "d2", "d2", "d3", "d3", "d3", "d3", "d4", "d4", "d4", "d4", "d5", "d5", "d5", "d5", "d6", "d6", "d6", "d6", "d7", "d7", "d7", "d7", "d8", "d8", "d8", "d8", "d9", "d9", "d9", "d9", "b1", "b1", "b1", "b1", "b2", "b2", "b2", "b2", "b3", "b3", "b3", "b3", "b4", "b4", "b4", "b4", "b5", "b5", "b5", "b5", "b6", "b6", "b6", "b6", "b7", "b7", "b7", "b7", "b8", "b8", "b8", "b8", "b9", "b9", "b9", "b9", "c1", "c1", "c1", "c1", "c2", "c2", "c2", "c2", "c3", "c3", "c3", "c3", "c4", "c4", "c4", "c4", "c5", "c5", "c5", "c5", "c6", "c6", "c6", "c6", "c7", "c7", "c7", "c7", "c8", "c8", "c8", "c8", "c9", "c9", "c9", "c9", "gd", "gd", "gd", "gd", "wd", "wd", "wd", "wd", "rd", "rd", "rd", "rd", "nw", "nw", "nw", "nw", "ew", "ew", "ew", "ew", "sw", "sw", "sw", "sw", "ww", "ww", "ww", "ww"};

char **deck;
size_t deck_size = 136;
char **discard_pile;
size_t discard_size = 0;
char **hand;
size_t hand_size = 0;


//for initializing starting deck
void initialize_deck() {
	for (int i = 0; i < deck_size; i++) {
		deck = realloc(deck, sizeof(char*) * (i+1));
		deck[i] = library[i];
	}
}

//for adding to the discard pile
void add_discard(char *new_discard) {
	discard_pile = realloc(discard_pile, sizeof(char*) * (discard_size+1));
	discard_pile[discard_size] = strndup(new_discard, strlen(new_discard));
	discard_size++;
}

//for removing tiles drawn by player and discarded by opponents
int remove_deck(char *tile_to_remove) {
	for (int i = 0; i < deck_size; i++) {
		if (strcmp(deck[i], tile_to_remove) == 0) {
			memmove(&deck[i], &deck[i+1], sizeof(char*) * (deck_size - (i + 1)));
			deck = realloc(deck, sizeof(char*)*(deck_size-1));
			deck_size--;
			return 0;
		}
	}
	printf("\nTile not in deck.\n");
	return 1;
}

//for adding a new tile to your hand and removing it from deck
int add_hand(char *new_tile) {
	for (int i = 0; i < deck_size; i++) {
		if (strcmp(deck[i], new_tile) == 0) {
			hand = realloc(hand, sizeof(char*)*(hand_size+1));
			hand[hand_size] = strndup(new_tile, strlen(new_tile));
			hand_size++;
			return remove_deck(new_tile);
		}
	}
	printf("\nThat is not one of the remaining tiles in the deck.\n");
	return 1;
}

//for removing tile discarded by player and adding it to discard
int remove_hand(char *tile_to_remove) {
	for (int i = 0; i < hand_size; i++) {
		if (strcmp(hand[i], tile_to_remove) == 0) {
			memmove(&hand[i], &hand[i+1], sizeof(char*) * (hand_size - (i + 1)));
			hand = realloc(hand, sizeof(char*)*(hand_size-1));
			hand_size--;
			add_discard(tile_to_remove);
			return 0;
		}
	}
	printf("\nTile not in hand.\n");
	return 1;
}

int main(int argc, char **argv) {
	int player_pos;
	char tile_drawn[2];
	initialize_deck();
	
	//get player pos
	printf("Enter player position: ");
	scanf("%d", &player_pos);
	while (player_pos < 1 || player_pos > 4) {
		printf("Invalid, must be between 1 and 4: ");
		scanf("%d", &player_pos);
	}
	
	
	//get starting hand
	printf("Enter your starting hand:");
	for (int i = 0; i < 13; i++) {
		scanf("%s", tile_drawn);
		int success = add_hand(tile_drawn);
		if (success != 0) {
			i--;
		}
	}
	
	//get discards if player isn't first
	if (player_pos > 1) {
		printf("What was discarded? ");
		for (int i = 0; i < player_pos-1; i++) {
			scanf("%s", tile_drawn);
			int success = remove_deck(tile_drawn);
			if (success != 0) {
				i--;
			}
			else {
				add_discard(tile_drawn);
			}
		}
	}
	
	//TODO: Enter loop for rounds starting with player's turn
	
	printf("\nHand: ");
	for (int i = 0; i < hand_size; i++) {
		printf("%s ", hand[i]);
	}
	remove_deck("c3");
	printf("\nDeck: ");
	for (int i = 0; i < deck_size; i++) {
		printf("%s ", deck[i]);
	}
	remove_hand("b2");
	printf("\nHand: ");
	for (int i = 0; i < hand_size; i++) {
		printf("%s ", hand[i]);
	}
	printf("\nDiscard: ");
	for (int i = 0; i < discard_size; i++) {
		printf("%s ", discard_pile[i]);
	}
  	printf("\n");
  	return 0;
}
