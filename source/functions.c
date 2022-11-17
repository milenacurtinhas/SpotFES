#include "libraries.h"

/**
 * @brief verifica se os nomes dos arquivos foram declarados via terminal e encerra o programa caso contrário
 */
void CheckDataFilesPath(int argc, char** argv) {
    ClearTerminal();
    if (argc == 1 || argc == 2) {
        PrintDataFilesError();
        exit(1);
    } else {
        char artists[STRING_BUFFER_SIZE], tracks[STRING_BUFFER_SIZE];

        sprintf(artists, "data/%s", argv[1]);
        sprintf(tracks, "data/%s", argv[2]);

        argv[1] = strdup(artists);
        argv[2] = strdup(tracks);
    }
}

void ClearTerminal() {
    while (system("clear") == 0) {
        break;
    }
}

/**
 * @brief alerta erro caso os arquivos de dados do spotify não sejam encontrados ou a pasta 'data' não exista
 */
void PrintDataFilesError() {
    RED_COLOUR;
    printf("• ERRO: Os arquivos de dados devem estar contidos na pasta 'data' e o programa deverá ser executado da seguinte forma:\n\n");
    printf("./spotfes nomedoarquivodosartistas.csv nomedoarquivodasmúsicas.csv\n\n");
    NORMAL_COLOUR;
}

/**
 * @brief alerta erro caso os arquivos binários não sejam encontrados ou a pasta 'binaries' não exista
 */
void PrintBinaryFilesError() {
    RED_COLOUR;
    printf("• ERRO: Verifique se a pasta 'binaries' existe e se ela está no mesmo diretório do programa.\n\n");
    NORMAL_COLOUR;
}

/**
 * @brief verifica se a função de ler os arquivos .csv chegou ao final
 * @param character o primeiro caractere da linha do arquivo
 * @return verdadeiro caso seja o fim do arquivo e falso caso contrário
 */
int EndOfFile(char character) {
    if (character == '\0' || character == '\n' || character == '\r') {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief verifica quantas vezes um valor específico ocorre com base em um caractere
 * @param character o caractere alvo
 * @param value_size o tamanho da string
 * @return a quantidade de um valor esperado
 */
int GetValueQuantity(char* character, int value_size) {
    int value_qty = 1;

    for (int m = 0; m < value_size; m++) {
        if (character[m] == '|') {
            value_qty++;
        }
    }

    return value_qty;
}

/**
 * @brief libera um bloco de memória previamente alocado e o atribui o ponteiro responsável como nulo
 * @param pointer um ponteiro qualquer
 */
void FreeAndNullPointer(void* pointer) {
    free(pointer);
    pointer = NULL;
}

void PrintMenuArt() {
    GREEN_COLOUR;
                                                                                                                                                                                                    
    printf("⠀⠀⠀⠀⠀⣀⣤⣶⣶⣿⣷⣶⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⢀⣾⣿⣿⣿⣿⡿⠿⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⣸⣿⠟⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣷⠀⠀⠀⣾⡿⠛⠻⢿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠀⢸⡿⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⣿⣿⣄⠀⣀⣤⣤⣶⣶⣶⣶⣦⣤⣄⡀⠀⠀⣿⣿⡆⠀⠀⣿⣇⠀⠀⠀⠀⠀⢸⣿⣤⡾⣿⣶⡀⠀⠀⣴⣾⣿⣷⣦⡀⠀⣿⣿⣿⡇⢰⣿⣿⣶⡆⢀⣶⡿⢿⣦⡀⠀⢠⣾⠿⣷⡆⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⣿⣿⣿⡿⠛⠋⠉⠉⠀⠀⠀⠉⠉⠛⠿⣷⣾⣿⣿⡇⠀⠀⠙⠿⣿⣷⣦⡀⠀⢸⣿⠁⠀⠀⢻⣷⠀⣸⡟⠁⠀⠈⢻⣧⠀⢸⣿⠀⠀⠀⢸⡏⠀⠀⣾⡏⠀⠀⢹⣷⠀⢺⣧⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⣿⣿⣿⣇⠀⣀⣠⣤⣤⣤⣤⣤⣄⡀⠀⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠉⢻⣿⠀⢸⣿⠀⠀⠀⢸⣿⠀⢿⣇⠀⠀⠀⢸⣿⠀⢸⣿⠀⠀⠀⢸⡇⠀⠀⣿⡟⠛⠛⠛⠛⠀⠈⠛⠿⣿⣆⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⢿⣿⣿⣿⡿⠛⠉⠉⠉⠉⠉⠉⠛⠻⣿⣿⣿⣿⣿⠁⠀⠠⣿⣦⣤⣤⣾⡟⠀⢸⣿⣧⣤⣤⣿⠏⠀⠘⣿⣦⣤⣴⣿⠇⠀⠸⣿⣤⡄⠀⢸⡇⠀⠀⠹⣷⣄⣀⣴⡄⠀⣴⣤⣀⣼⡿⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠸⣿⣿⣿⣄⣠⣤⣴⣶⣶⣶⣤⣄⡀⣸⣿⣿⣿⡏⠀⠀⠀⠈⠉⠛⠛⠉⠀⠀⢸⣿⠀⠙⠛⠉⠀⠀⠀⠈⠙⠛⠋⠁⠀⠀⠀⠙⠛⠁⠀⠈⠉⠀⠀⠀⠈⠛⠛⠉⠀⠀⠈⠙⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⣿⣿⡿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");

    NORMAL_COLOUR;
    printf("Ｍｉｌｅｎａ Ｃｕｒｔｉｎｈａｓ ｜ Ｍｉｓａｅｌ Ｆｌｏｒｅｎｃｉａｎｏ\n\n");
    BLACK_COLOUR;
}

/**
 * @brief lê um número inteiro e verifica se está contido entre um intervalo definido pelos parâmetros
 * @param min_range o menor valor de um intervalo fechado
 * @param max_range o maior valor de um intervalo fechado
 * @return um número válido ou uma mensagem de erro pedindo o input novamente
 */
int GetValidIntegerInput(int min_range, int max_range) {
    while (1) {
        char buffer[STRING_BUFFER_SIZE];

        if (fgets(buffer, STRING_BUFFER_SIZE, stdin) != NULL) {
            if (buffer[0] >= '0' && buffer[0] <= '9') {
                int input = atoi(buffer);

                if (input >= min_range && input <= max_range) {
                    return input;
                }
            }
        }

        RED_COLOUR;
        printf("• ERRO: Opção inválida. Tente novamente: ");
    }
}

/**
 * @brief lê um caracter e verifica se é uma resposta sim/não válida
 * @return verdadeiro caso a resposta seja sim e falso caso a resposta seja não
 */
char GetValidYesOrNoInput() {
    while (1) {
        char input[STRING_BUFFER_SIZE];

        if (fgets(input, STRING_BUFFER_SIZE, stdin) != NULL) {
            if (input[0] == 'N' || input[0] == 'n') {
                return 0;
            } else if (input[0] == 'S' || input[0] == 's') {
                return 1;
            }
        }

        RED_COLOUR;
        printf("• ERRO: Opção inválida. Tente novamente: ");
        NORMAL_COLOUR;
    }
}

/**
 * @brief muda os caracteres para minúsculo para permitir comparações que não diferenciem maiúsculas de minúsculas (case insensitive)
 * @param string string a ser transformada
 * @return string com todos os caracteres minúsculos
 */
char* GetLowcaseString(char* string) {
    for (int m = 0; string[m] != '\0'; m++) {
        string[m] = tolower(string[m]);
    }

    return string;
}

/**
 * @brief separa duração da música em minutos e segundos e imprime
 * @param duration_ms duração em milisegundos
 */
void PrintTrackLength(int duration_ms) {
    int duration_s = duration_ms / 1000;
    int minutes = duration_s / 60;
    int seconds = duration_s % 60;

    printf("Duração: %02d:%02d\n", minutes, seconds);
}

/**
 * @brief imprime uma data no formato convencional
 */
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

/**
 * @brief verifica e imprime se a música possui conteúdo explícito
 * @param explicit valor booleano sobre a característica da música
 */
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

/**
 * @brief calcula distância euclidiana
 * @param features valor das característica de uma música
 * @param averages média dos valores das características de todas as músicas na playlist
 * @return distância euclidiana entre os valores passados como parâmetro
 */
float CalculateEuclideanDistance(float* features, float* averages) {
    float sum = 0;

    for (int m = 0; m < 8; m++) {
        sum += pow((features[m] - averages[m]), 2);
    }

    return sqrt(sum);
}