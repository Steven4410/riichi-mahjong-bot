#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "tiles.h"

char **deck;
size_t deck_size = 136;
char **discard_pile;
size_t discard_size = 0;
char **hand;
size_t hand_size = 0;

void clear_input_stream() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


//for initializing starting deck
void initialize_deck() {
	for (int i = 0; i < deck_size; i++) {
		deck = realloc(deck, sizeof(char*) * (i+1));
		deck[i] = strndup(library[i], strlen(library[i])+1);
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
			//Sort tile into hand
			int new_tile_value = 0;
			//get tile library index
			for (int j = 0; j < 34; j++) {
				if (strcmp(library_simple[j], new_tile) == 0) {
					new_tile_value = j;
					break;
				}
			}
			// find place in hand
			for (int cur_tile = 0; cur_tile < hand_size; cur_tile++) {
				int cur_tile_value = 0;
				//get tile in hand's library index
				for (int j = 0; j < 34; j++) {
					if (strcmp(library_simple[j], hand[cur_tile]) == 0) {
						cur_tile_value = j;
						break;
					}
				}
				// if the tile in hand's index is greater or equal to new one, put new before it in hand
				if (new_tile_value <= cur_tile_value) {
					hand = realloc(hand, sizeof(char*)*(hand_size+1));
					memmove(&hand[cur_tile+1], &hand[cur_tile], sizeof(char*) * (hand_size - cur_tile ));
					hand[cur_tile] = strndup(new_tile, strlen(new_tile)+1);
					hand_size++;
					return remove_deck(new_tile);
				}
			}
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
	clear_input_stream();
	
	
	//get starting hand
	printf("Enter your starting hand:");
	for (int i = 0; i < 13; i++) {
		scanf("%s", tile_drawn);
		int success = add_hand(tile_drawn);
		if (success != 0) {
			i--;
		}
	}
	clear_input_stream();
	
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
		clear_input_stream();
	}
	
	//TODO: Enter loop for rounds starting with player's turn
	char quit_choice = 'y';
	bool riichi = false;
	bool victory = false;
	
	while (tolower(quit_choice) != 'n') {
		int success = -1;
		while (success != 0) {
			printf("Enter your draw: ");
			scanf("%s", tile_drawn);
			success = add_hand(tile_drawn);
		}
		clear_input_stream();
		
		printf("Current Hand:");
		for (int i = 0; i < hand_size; i++) {
			printf(" %s", hand[i]);
		}
		printf("\n");
		
		success = -1;
		
		//Victory check function?
		
		if (victory == true) {
			printf("You have a winning hand! Run program to play again.\n");
		}
		
		//TODO: get best discard choice.
		printf("You should discard \"\"\n");
		while (success != 0) {
			printf("Enter your discard: ");
			scanf("%s", tile_drawn);
			success = remove_hand(tile_drawn);
		}
		clear_input_stream();
		
		//TODO: Riichi  check function?
		if (riichi == true) {
			printf("Declare Riichi!");
		}
		
		printf("\nOpponents discard: ");
		for (int i = 0; i < 3; i++) {
			scanf("%s", tile_drawn);
			success = remove_deck(tile_drawn);
			if (success != 0) {
				i--;
			}
			else {
				add_discard(tile_drawn);
			}
		}
		clear_input_stream();
		printf("Continue? (y/n): ");
		scanf("%c", &quit_choice);
		clear_input_stream();
	}
	printf("\nQuiting Program\n");
  	return 0;
}
