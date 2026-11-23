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
			free(deck[i]);
			memmove(&deck[i], &deck[i+1], sizeof(char*) * (deck_size - (i + 1)));
			deck = realloc(deck, sizeof(char*) * (deck_size - 1));
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
			free(hand[i]);
			memmove(&hand[i], &hand[i+1], sizeof(char*) * (hand_size - (i + 1)));
			hand = realloc(hand, sizeof(char*) * (hand_size - 1));
			hand_size--;
			add_discard(tile_to_remove);
			return 0;
		}
	}
	printf("\nTile not in hand.\n");
	return 1;
}

// For removing tiles from hand_copy
void remove_copy_tile(char **hand_copy, size_t *hand_copy_size, int index) {
	free(hand_copy[index]);
	memmove(&hand_copy[index], &hand_copy[index + 1], sizeof(char*) * (*hand_copy_size - (index + 1)));
	(*hand_copy_size)--;
}

// Furiten checker
bool is_in_discard(const char *tile) {
	for (int p = 0; p < discard_size; p++) {
		if (strcmp(discard_pile[p], tile) == 0) {
			return true;
		}
	}
	return false;
}

// Determines best discard
bool discard() {
	size_t melds = 0;
	size_t pairs = 0;
	size_t partial_melds = 0;
	bool victory = false;

	// Create duplicate hand
	size_t hand_copy_size = hand_size;
	char **hand_copy = malloc(sizeof(char*) * hand_copy_size);
	for (int i = 0; i < hand_copy_size; i++) {
		hand_copy[i] = strdup(hand[i]);
	}
	
	// Compare each tile in hand_copy with the first index of each sequence
	for (int i = 0; i < hand_copy_size; i++) {
		for (int j = 0; j < 24; j++) {
			if (strcmp(hand_copy[i], sequences[j][0]) == 0) {
				bool matched = false;
				// First tile matched
				for (int k = i + 1; k < hand_copy_size && !matched; k++) {
					if (strcmp(hand_copy[k], sequences[j][1]) == 0) {
						// 2nd tile matched
						for (int m = k + 1; m < hand_copy_size; m++) {
							if (strcmp(hand_copy[m], sequences[j][2]) == 0) {
								// Sequence matched (remove in reverse index order)
								remove_copy_tile(hand_copy, &hand_copy_size, m);
								remove_copy_tile(hand_copy, &hand_copy_size, k);
								remove_copy_tile(hand_copy, &hand_copy_size, i);

								i--;
								matched = true;
								melds++;
								break;
							}
						}
					}
				}
				break;
			}
		}
	}

	// Compare each tile in hand_copy with the first index of each triplet
	for (int i = 0; i < hand_copy_size; i++) {
		for (int j = 0; j < 34; j++) {
			if (strcmp(hand_copy[i], triplets[j][0]) == 0) {
				bool matched = false;
				// First tile matched
				for (int k = i + 1; k < hand_copy_size && !matched; k++) {
					if (strcmp(hand_copy[k], triplets[j][1]) == 0) {
						// 2nd tile matched
						for (int m = k + 1; m < hand_copy_size; m++) {
							if (strcmp(hand_copy[m], triplets[j][2]) == 0) {
								// Triplet matched (remove in reverse index order)
								remove_copy_tile(hand_copy, &hand_copy_size, m);
								remove_copy_tile(hand_copy, &hand_copy_size, k);
								remove_copy_tile(hand_copy, &hand_copy_size, i);

								i--;
								matched = true;
								melds++;
								break;
							}
						}
					}
				}
				break;
			}
		}
	}

	int min_pair_copies_in_deck = 5;
	const char *worst_pair_value = NULL;

	// Check for pairs
	for (int i = 0; i < hand_copy_size; i++) {
		for (int j = 0; j < 34; j++) {
			if (strcmp(hand_copy[i], triplets[j][0]) == 0) {
				// 1st tile matched
				for (int k = i + 1; k < hand_copy_size; k++) {
					if (strcmp(hand_copy[k], triplets[j][1]) == 0) {
						// Pair found
						// Furiten check
						int pair_copies_in_deck = 0;
						if (!is_in_discard(triplets[j][2])) {
							for (int d = 0; d < deck_size; d++) {
								if (strcmp(deck[d], triplets[j][2]) == 0) {
									pair_copies_in_deck++;
								}
							}
						}
						if (pair_copies_in_deck <= min_pair_copies_in_deck) {
							min_pair_copies_in_deck = pair_copies_in_deck;
							worst_pair_value = triplets[j][0];
						}

						remove_copy_tile(hand_copy, &hand_copy_size, k);
						remove_copy_tile(hand_copy, &hand_copy_size, i);

						i--;
						pairs++;
						if (pairs > 1) {
						    partial_melds++;
						}
						break;
					}
				}
				break;
			}
		}
	}

	int min_seq_copies_in_deck = 9; // One more than max
	const char *worst_sequence_value = NULL;

	// Check for connected partial sequences
	for (int i = 0; i < hand_copy_size; i++) {
		for (int j = 0; j < 24; j++) {
			if (strcmp(hand_copy[i], sequences[j][0]) == 0) {
				// 1st tile matched
				for (int k = i + 1; k < hand_copy_size; k++) {
					if (strcmp(hand_copy[k], sequences[j][1]) == 0) {
						// Partial sequence found
						int seq_copies_in_deck = 0;
						bool end_discarded = is_in_discard(sequences[j][2]);
						bool start_discarded = is_in_discard(sequences[j][3]);

						for (int d = 0; d < deck_size; d++) {
							if ((!end_discarded && strcmp(deck[d], sequences[j][2]) == 0) ||
							    (!start_discarded && strcmp(deck[d], sequences[j][3]) == 0)) {
								seq_copies_in_deck++;
							}
						}
						if (seq_copies_in_deck < min_seq_copies_in_deck) {
							min_seq_copies_in_deck = seq_copies_in_deck;
							worst_sequence_value = sequences[j][0];
						}

						remove_copy_tile(hand_copy, &hand_copy_size, k);
						remove_copy_tile(hand_copy, &hand_copy_size, i);

						i--;
						partial_melds++;
						break;
					}
				}
				break;
			}
		}
	}

	// Check for gapped partial sequences
	for (int i = 0; i < hand_copy_size; i++) {
		for (int j = 0; j < 24; j++) {
			if (strcmp(hand_copy[i], sequences[j][0]) == 0) {
				// 1st tile matched
				for (int k = i + 1; k < hand_copy_size; k++) {
					if (strcmp(hand_copy[k], sequences[j][2]) == 0) {
						// Gapped sequence found
						int gap_copies_in_deck = 0;
						if (!is_in_discard(sequences[j][1])) {
							for (int d = 0; d < deck_size; d++) {
								if (strcmp(deck[d], sequences[j][1]) == 0) {
									gap_copies_in_deck++;
								}
							}
						}
						if (gap_copies_in_deck < min_seq_copies_in_deck) {
							min_seq_copies_in_deck = gap_copies_in_deck;
							worst_sequence_value = sequences[j][0];
						}

						remove_copy_tile(hand_copy, &hand_copy_size, k);
						remove_copy_tile(hand_copy, &hand_copy_size, i);

						i--;
						partial_melds++;
						break;
					}
				}
				break;
			}
		}
	}

	int min_single_copies_in_deck = 5;
	const char *worst_single_value = NULL;

	// Check for singles
	for (int i = 0; i < hand_copy_size; i++) {
		int single_copies_in_deck = 0;
		for (int d = 0; d < deck_size; d++) {
			if (strcmp(deck[d], hand_copy[i]) == 0) {
				single_copies_in_deck++;
			}
		}
		// Update if this single has fewer tiles remaining in deck
		if (single_copies_in_deck <= min_single_copies_in_deck) {
			min_single_copies_in_deck = single_copies_in_deck;
			worst_single_value = hand_copy[i];
		}
	}


	if (melds == 4 && pairs == 1) {
		printf("Declare Victory!\n");
		victory = true;
	} else {
		if (worst_single_value != NULL) {
			printf("You should discard \"%s\", since it is not part of any pair or sequence and can only be turned into a pair by %d tiles.\n", worst_single_value, min_single_copies_in_deck);
		} else if (worst_sequence_value != NULL) {
			printf("You should discard \"%s\", since you have no standalone tiles and this sequence can only be completed by %d tiles.\n", worst_sequence_value, min_seq_copies_in_deck);
		} else if (worst_pair_value != NULL) {
			printf("You should discard \"%s\", since you have no standalone tiles and this pair can only be completed by %d tiles.\n", worst_pair_value, min_pair_copies_in_deck);
		}
		if ((melds == 4 && pairs == 0) || (melds == 3 && partial_melds == 1 && pairs >= 1)) {
			printf("Declare Riichi, you are one tile away from victory.\n");
			printf("Steal the first tile that completes your hand.\n");
			victory = true;
		}
	}

	// Frees duplicate hand
	for (int i = 0; i < hand_copy_size; i++) {
		free(hand_copy[i]);
	}
	free(hand_copy);

	return victory;
}

int main(int argc, char **argv) {
	char play_again = 'y';

	while (tolower(play_again) != 'n') {
		int player_pos;
		char tile_drawn[3];

		deck = NULL;
		deck_size = 136;
		discard_pile = NULL;
		discard_size = 0;
		hand = NULL;
		hand_size = 0;

		initialize_deck();
		
		//get player pos
		printf("Enter player position: ");
		scanf("%d", &player_pos);
		while (player_pos < 1 || player_pos > 4) {
			printf("Invalid, must be between 1 and 4: ");
			scanf("%d", &player_pos);
		}
		clear_input_stream();
		
		//get discards if player isn't first
		if (player_pos > 1) {
			printf("\nEnter Your Opponents' discard (One tile at a time): ");
			for (int i = 0; i < player_pos - 1; i++) {
				scanf("%2s", tile_drawn);
				int success = remove_deck(tile_drawn);
				if (success != 0) {
					i--;
				}
			}
			clear_input_stream();
		}
		
		//get starting hand
		for (int i = 0; i < 13; i++) {
			printf("Enter your starting hand (one tile at a time): ");
			scanf("%2s", tile_drawn);
			int success = add_hand(tile_drawn);
			if (success != 0) {
				i--;
			}
		}
		clear_input_stream();
		
		//Enter loop for rounds starting with player's turn
		char quit_choice = 'y';
		
		while (tolower(quit_choice) != 'n') {
			if (deck_size == 0) {
				printf("Deck is empty\n");
				break;
			}

			int success = -1;
			while (success != 0) {
				printf("Enter your draw: ");
				scanf("%2s", tile_drawn);
				success = add_hand(tile_drawn);
			}
			clear_input_stream();
			
			printf("Current Hand: ");
			for (int i = 0; i < hand_size; i++) {
				printf(" %s", hand[i]);
			}
			printf("\n");
			
			// Check for victory or get best discard choice
			if (discard()) {
				break;
			}

			success = -1;
			while (success != 0) {
				printf("Enter your discard: ");
				scanf("%2s", tile_drawn);
				success = remove_hand(tile_drawn);
			}
			clear_input_stream();
			
			bool deck_empty = false;
			for (int i = 0; i < 3; i++) {
				if (deck_size == 0) {
					printf("\nDeck is empty\n");
					deck_empty = true;
					break;
				}
				printf("\nEnter Your Opponents' discard (One at a time): ");
				scanf("%2s", tile_drawn);
				success = remove_deck(tile_drawn);
				if (success != 0) {
					i--;
				}
			}
			if (deck_empty) {
				break;
			}

			clear_input_stream();
			printf("Continue round? (y/n): ");
			scanf(" %c", &quit_choice);
			clear_input_stream();
		}

		// Free deck, hand, and discard
		for (size_t i = 0; i < deck_size; i++) {
			free(deck[i]);
		}
		free(deck);

		for (size_t i = 0; i < hand_size; i++) {
			free(hand[i]);
		}
		free(hand);

		for (size_t i = 0; i < discard_size; i++) {
			free(discard_pile[i]);
		}
		free(discard_pile);

		printf("\nPlay again? (y/n): ");
		scanf(" %c", &play_again);
		clear_input_stream();
		printf("\n");
	}

	printf("Quiting Program\n");
	return 0;
}
