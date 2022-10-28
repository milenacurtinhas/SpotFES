#include "libraries.h"

struct spotfes {
    tTracks** tracks;
    int tracks_qty;
    tArtists** artists;
    int artists_qty;
};

void PrintsMainMenu() {
    printf("SPOTFES by M&M\n\n");
    printf("1 - Buscar músicas\n");
    printf("2 - Listar uma música\n");
    printf("3 - Criar uma playlist\n");
    printf("4 - Listar playlists\n");
    printf("5 - Listar uma playlist\n");
    printf("6 - Adicionar uma música na playlist\n");
    printf("7 - Recomendar músicas parecidas com uma playlist\n");
    printf("8 - Gerar relatório\n\n");
}

int GetsUserInput(int input) {
    printf("Digite a opção desejada: ");
    scanf("%d%*c", &input);

    return input;
}