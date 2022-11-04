#include "libraries.h"

struct tspotfes {
    tArtists** artists;
    int* artists_qty;
    tTracks** tracks;
    int* tracks_qty;
    tPlaylists** playlists;
    int* playlists_qty;
};

void CheckDataFilesPath(int argc) {
    switch (argc) {
        case 1:
            printf("ERRO: Nenhum arquivo de dados do Spotify foi declarado. Verifique se eles estão contidos na pasta 'data', no mesmo diretório desse programa.\n");
            exit(1);
            break;
        case 2:
            printf("ERRO: Um dos arquivos de dados do Spotify não foi declarado. Verifique se ele está contido na pasta 'data', no mesmo diretório desse programa.\n");
            exit(1);
            break;
        case 3:
            break;
    }
}

tSpotfes* AllocateSpotfes() {
    tSpotfes* spotfes = (tSpotfes*)malloc(sizeof(tSpotfes));

    spotfes->artists = (tArtists**)malloc(sizeof(tArtists*) * 128);
    spotfes->artists_qty = (int*)malloc(sizeof(int));

    spotfes->tracks = (tTracks**)malloc(sizeof(tTracks*) * 128);
    spotfes->tracks_qty = (int*)malloc(sizeof(int));

    // spotfes->playlists = (tPlaylists**)malloc(sizeof(tPlaylists*) * 16); // ativar quando a função das playlists estiverem prontas
    // spotfes->playlists_qty = (int*)malloc(sizeof(int)); // ativar quando a função das playlists estiverem prontas

    for (int m = 0; m < 128; m++) {
        spotfes->artists[m] = AllocateArtists();
        spotfes->tracks[m] = AllocateTracks();
        /*
        if (m < 16) {
            spotfes->playlists[m] = AllocatePlaylists(); // ativar quando a função das playlists estiverem prontas
        }
        */
    }

    return spotfes;
}

void FreeUpSpotfes(tSpotfes* spotfes) {
    for (int m = 0; m < *spotfes->artists_qty; m++) {
        FreeUpArtists(spotfes->artists[m]);
    }

    for (int m = 0; m < *spotfes->tracks_qty; m++) {
        FreeUpTracks(spotfes->tracks[m]);
    }
    /*
    for (int m = 0; m < *spotfes->playlists_qty; m++) { // ativar quando a função das playlists estiverem prontas
        FreeUpPlaylists(spotfes->playlists[m]);
    }
    */

    FreeAndNullPointer(spotfes->artists);
    FreeAndNullPointer(spotfes->artists_qty);

    FreeAndNullPointer(spotfes->tracks);
    FreeAndNullPointer(spotfes->tracks_qty);

    // FreeAndNullPointer(spotfes->playlists); // ativar quando a função das playlists estiverem prontas
    // FreeAndNullPointer(spotfes->playlists_qty); // ativar quando a função das playlists estiverem prontas

    FreeAndNullPointer(spotfes);
}

void ReadSpotifyDataFiles(tSpotfes* spotfes, char** argv) {
    FILE* artists_data = fopen(argv[1], "r");
    if (!artists_data) {
        printf("ERRO: Não foi possível acessar o arquivo de dados dos artistas. Verifique se ele está no mesmo diretório do executável desse programa.\n");
        exit(1);
    }

    spotfes->artists = ReadArtistsDataFiles(spotfes->artists, artists_data, spotfes->artists_qty);
    fclose(artists_data);

    FILE* tracks_data = fopen(argv[2], "r");
    if (!tracks_data) {
        printf("ERRO: Não foi possível acessar o arquivo de dados das músicas. Verifique se ele está no mesmo diretório do executável desse programa.\n");
        exit(1);
    }

    spotfes->tracks = ReadTracksDataFiles(spotfes->tracks, tracks_data, spotfes->tracks_qty);
    LinkArtistsToTracks(spotfes, spotfes->tracks, spotfes->artists);

    fclose(tracks_data);
}

int GetArtistsQuantity(tSpotfes* spotfes) {
    return (*spotfes->artists_qty);
}

int GetTracksQuantity(tSpotfes* spotfes) {
    return (*spotfes->tracks_qty);
}

int SetUpMainMenu(int input) {
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