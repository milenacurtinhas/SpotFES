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

int GetValueQuantity(char* character, int value_size) {
    int value_qty = 1;

    for (int m = 0; m < value_size; m++) {
        if (character[m] == '|') {
            value_qty++;
        }
    }

    return value_qty;
}

int LessArtistsThanMallocs(int quantity, int mallocs) {
    return quantity < mallocs;
}

int SameID(char* source_id, char* target_id) {
    int result = strcmp(source_id, target_id);

    if (result == 0) {
        return 1;
    } else {
        return 0;
    }
}