#include "libraries.h"

void PrintMissingFilesErrorAndQuitProgram() {
    printf("• ERRO: Os arquivos de dados devem estar contidos na pasta 'data' e o programa deverá ser executado da seguinte forma:\n\n");
    printf("./SpotFES NomeDoArquivoDosArtistas.csv NomeDoArquivoDasMúsicas.csv\n\n");
    exit(1);
}

void CheckDataFilesPath(int argc, char** argv) {  // ESSE WARNING VAI SUMIR QUANDO FIZERMOS AS FUNÇÕES ABAIXO
    ClearTerminal();
    if (argc == 1 || argc == 2) {
        PrintMissingFilesErrorAndQuitProgram();
    } else {
        char artists[512], tracks[512];

        sprintf(artists, "data/%s", argv[1]);
        sprintf(tracks, "data/%s", argv[2]);

        argv[1] = strdup(artists);
        argv[2] = strdup(tracks);
    }
}

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

int GetValidIntegerInput(int min_range, int max_range) {
    while (1) {
        char buffer[16];

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (buffer[0] >= '0' && buffer[0] <= '9') {
                int input = atoi(buffer);

                if (input >= min_range && input <= max_range) {
                    return input;
                }
            }
        }

        printf("• ERRO: Opção inválida. Tente novamente: ");
    }
}

char GetValidYesOrNoInput() {
    while (1) {
        char input[16];

        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (input[0] == 'N' || input[0] == 'n') {
                return 0;
            } else if (input[0] == 'S' || input[0] == 's') {
                return 1;
            }
        }

        printf("• ERRO: Opção inválida. Tente novamente: ");
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
        if (month) {
            printf("%02d/", month);
            if (year) {
                printf("%04d", year);
            }
        }
    } else if (!day && !month && !year) {
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
    float euclideanDistance = 0, sum = 0;

    for (int m = 0; m < 8; m++) {
        sum += pow((features[m] - averages[m]), 2);
    }

    euclideanDistance = sqrt(sum);

    return euclideanDistance;
}

void ClearTerminal() {
    while (system("clear") == 0) {
        break;
    }
}