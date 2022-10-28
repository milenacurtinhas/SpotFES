#include "libraries.h"

struct tspotfes {
    tArtists** artists;
    int artists_qty;
    tTracks** tracks;
    int tracks_qty;
    tPlaylists** playlists;
    int playlists_qty;
};

void CheckDataFilesPath(int argc) {
    if (argc < 3) {
        printf("ERRO: Não foi possível acessar os arquivos de dados do Spotify.\n");
        exit(1);
    }
}

tSpotfes* AllocateSpotfes() {
    tSpotfes* spotfes = (tSpotfes*)malloc(sizeof(tSpotfes));
    spotfes->artists = (tArtists**)malloc(sizeof(tArtists*) * 128);
    spotfes->tracks = (tTracks**)malloc(sizeof(tTracks*) * 128);
    spotfes->playlists = (tPlaylists**)malloc(sizeof(tPlaylists*) * 16);

    for (int m = 0; m < 128; m++) {
        spotfes->artists[m] = AllocateArtists();
        spotfes->tracks[m] = AllocateTracks();
        if (m < 16) {
            spotfes->playlists[m] = AllocatePlaylists();
        }
    }

    spotfes->artists_qty = 0;
    spotfes->tracks_qty = 0;
    spotfes->playlists_qty = 0;

    return spotfes;
}

void ReadSpotifyData(tSpotfes* spotfes, char** argv) {
    FILE* artists = fopen(argv[1], "r");
    if (!artists) {
        printf("ERRO: Não foi possível acessar o arquivo de dados dos artistas.\n");
        exit(1);
    }

    FILE* tracks = fopen(argv[2], "r");
    if (!tracks) {
        printf("ERRO: Não foi possível acessar o arquivo de dados das músicas.\n");
        exit(1);
    }

    fclose(artists);
    fclose(tracks);
}

int SetUpMainMenu() {
    int input;

    printf("SPOTFES by M&M\n\n");
    printf("1 - Buscar músicas\n");
    printf("2 - Listar uma música\n");
    printf("3 - Criar uma playlist\n");
    printf("4 - Listar playlists\n");
    printf("5 - Listar uma playlist\n");
    printf("6 - Adicionar uma música na playlist\n");
    printf("7 - Recomendar músicas parecidas com uma playlist\n");
    printf("8 - Gerar relatório\n\n");
    printf("Digite a opção desejada: ");
    scanf("%d%*c", &input);

    return input;
}

void FreeUpSpotfes(tSpotfes* spotfes) {
    for (int m = 0; m < spotfes->artists_qty; m++) {
        FreeUpArtists(spotfes->artists[m]);
    }

    for (int m = 0; m < spotfes->tracks_qty; m++) {
        FreeUpTracks(spotfes->tracks[m]);
    }

    for (int m = 0; m < spotfes->playlists_qty; m++) {
        FreeUpPlaylists(spotfes->playlists[m]);
    }

    free(spotfes->artists);
    free(spotfes->tracks);
    free(spotfes->playlists);
    free(spotfes);
}