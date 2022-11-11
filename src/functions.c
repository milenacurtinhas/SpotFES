#include "libraries.h"

void FreeAndNullPointer(void* pointer) {
    free(pointer);
    pointer = NULL;
}

int EndOfFile(char character) {
    if (character == '\0' || character == '\n' || character == '\r') {
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
    int duration_s = duration_ms / 1000;
    int minutes = duration_s / 60;
    int seconds = duration_s % 60;

    printf("Duração: %02d:%02d\n", minutes, seconds);
}

void PrintTrackReleaseDate(int day, int month, int year) {
    printf("Data de lançamento: ");

    if (day) {
        printf("%02d/", day);
    }

    if (month) {
        printf("%02d/", month);
    }

    if (year) {
        printf("%04d", year);
    }

    if (!day && !month && !year) {
        printf("não informada");
    }

    printf("\n");
}

void PrintTrackExplicitInfo(int explicit) {
    switch (explicit) {
        case 0:
            printf("Não possui conteúdo inapropriado para menores de idade\n");
            break;
        case 1:
            printf("Possui conteúdo inapropriado para menores de idade\n");
            break;
    }
}

float CalculateEuclideanDistance(float* features, float* averages) {
    int euclideanDistance = 0;

    for (int m = 0; m < 8; m++) {
        euclideanDistance += pow((features[m] = averages[m]), 2);
    }

    return euclideanDistance;
}