#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// create tile node that stores data 
struct TileNode {
    char label[2];
    int rank;
    struct TileNode* left;
    struct TileNode* right;
};

// Create new tile node labeling the tile node with the input tile
struct TileNode* newTile(char* tilestr) {
    struct TileNode* tile = (struct TileNode*)malloc(sizeof(struct TileNode));
    tile->label[0] = tilestr[0];
    tile->label[1] = tilestr[1];
    switch (tile->label[1]){
        case 'd': // add a multiple of 10 to differentiate the ranks of suits
            tile->rank = tile->label[0] - '0'; // works on dragons as well, each dragon has its own big number.
            // white dragon = 71; green dragon = 55; red dragon = 66
            break;
        case 'b':
            tile->rank = tile->label[0] - '0' + 10;
            break;
        case 'c':
            tile->rank = tile->label[0] - '0' + 20;
            break;
        case 'w': // winds jump rank by two so that they are not seen as sequential in the tree
            if (tile->label[0] == 'e')
                tile->rank = 30;
            else if (tile->label[0] == 's')
                tile->rank = 32;
            else if (tile->label[0] == 'w')
                tile->rank = 34;
            else if (tile->label[0] == 'n')
                tile->rank = 36;
            else
                tile->rank = 0; // zero out tile rank signifying that the tile is invalid
            break;
           
    }
    tile->left = tile->right = NULL;
    return tile;
}

int main(){
    struct TileNode* red_dragon = newTile("1b");
    printf("%d\n", red_dragon->rank);
}
