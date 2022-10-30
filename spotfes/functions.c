#include "libraries.h"

void FreeAndNullPointer(void* pointer) {
    free(pointer);
    pointer = NULL;
}

int EndOfFile(char character) {
    if (character == 0 || character == 10 || character == 13) {
        return 1;
    } else {
        return 0;
    }
}

int GetGenresQuantity(char* character, int genres_size) {
    int genres_qty = 1;

    for (int m = 0; m < genres_size; m++) {
        if (character[m] == '|') {
            genres_qty++;
        }
    }

    return genres_qty;
}