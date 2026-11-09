#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *library[] = {"d1", "d1", "d1", "d1", "d2", "d2", "d2", "d2", "d3", "d3", "d3", "d3", "d4", "d4", "d4", "d4", "d5", "d5", "d5", "d5", "d6", "d6", "d6", "d6", "d7", "d7", "d7", "d7", "d8", "d8", "d8", "d8", "d9", "d9", "d9", "d9", "b1", "b1", "b1", "b1", "b2", "b2", "b2", "b2", "b3", "b3", "b3", "b3", "b4", "b4", "b4", "b4", "b5", "b5", "b5", "b5", "b6", "b6", "b6", "b6", "b7", "b7", "b7", "b7", "b8", "b8", "b8", "b8", "b9", "b9", "b9", "b9", "c1", "c1", "c1", "c1", "c2", "c2", "c2", "c2", "c3", "c3", "c3", "c3", "c4", "c4", "c4", "c4", "c5", "c5", "c5", "c5", "c6", "c6", "c6", "c6", "c7", "c7", "c7", "c7", "c8", "c8", "c8", "c8", "c9", "c9", "c9", "c9", "gd", "gd", "gd", "gd", "wd", "wd", "wd", "wd", "rd", "rd", "rd", "rd", "nw", "nw", "nw", "nw", "ew", "ew", "ew", "ew", "sw", "sw", "sw", "sw", "ww", "ww", "ww", "ww"};
size_t deck_size = 136;
char **deck;
char **discard_pile;
size_t discard_size = 0;
char **hand;


void initialize_deck() {
	for (int i = 0; i < deck_size; i++) {
		deck = realloc(deck, sizeof(char*) * (i+1));
		deck[i] = library[i];
	}
}

void add_discard(char *new_discard) {
	discard_pile = realloc(discard_pile, sizeof(discard_pile) + sizeof(char*));
	discard_pile[discard_size] = new_discard;
	discard_size++;
}

int remove_deck(char *removed_tile) {
	for (int i = 0; i < deck_size; i++) {
		printf("\n%s", deck[i]);
		if (strcmp(deck[i], removed_tile) == 0) {
			memmove(&deck[i], &deck[i+1], sizeof(char*) * (deck_size - (i + 1)));
			deck = realloc(deck, sizeof(char*)*(deck_size-1));
			deck_size--;
			printf("\n%s", deck[i]);
			return 0;
		}
	}
	printf("\nTile not in deck");
	return 1;
}

int main(int argc, char **argv) {
	initialize_deck();
  	printf("\n");
  	return 0;
}
