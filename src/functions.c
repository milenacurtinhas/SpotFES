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

char* GetLowcaseString(char* string) {
    for (int m = 0; string[m] != '\0'; m++) {
        string[m] = tolower(string[m]);
    }

    return string;
}

void PrintTrackLength(int duration_ms) {
    int duration = duration_ms / 1000;
    int minutes = duration / 60;
    int seconds = duration % 60;

    printf("Duração: ");
    if (!minutes) {
        if (!seconds) {
            printf("N/A\n");
        } else if (seconds == 1) {
            printf("1 segundo\n");
        } else {
            printf("%d segundos\n", seconds);
        }
    } else if (minutes == 1) {
        if (!seconds) {
            printf("1 minuto\n");
        } else if (seconds == 1) {
            printf("1 minuto e 1 segundo\n");
        } else {
            printf("1 minuto e %d segundos\n", seconds);
        }
    } else if (minutes > 1) {
        if (!seconds) {
            printf("%d minutos\n", minutes);
        } else if (seconds == 1) {
            printf("%d minutos e 1 segundo\n", minutes);
        } else {
            printf("%d minutos e %d segundos\n", minutes, seconds);
        }
    }
}

void PrintExplicitInfo(int explicit) {
    switch (explicit) {
        case 0:
            printf("Não contém conteúdo inapropriado para menores de idade\n");
            break;
        case 1:
            printf("Contém conteúdo inapropriado para menores de idade\n");
            break;
    }
}

/*float CalculateEuclideanDistance(float* features, float* averages) {
    int euclideanDistance = 0;

    for (int m = 0; m < 8; m++) {
        euclideanDistance += pow((features[m] = averages[m]), 2);
    }

    return euclideanDistance;
}*/