#include "libraries.h"

struct tspotfes {
    tArtists** artists;
    int* artists_qty;
    tTracks** tracks;
    int* tracks_qty;
    tPlaylists** playlists;
    int* playlists_qty;
    int* playlists_allocs;
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
    (*spotfes->artists_qty) = 0;

    spotfes->tracks = (tTracks**)malloc(sizeof(tTracks*) * 128);
    spotfes->tracks_qty = (int*)malloc(sizeof(int));
    (*spotfes->tracks_qty) = 0;

    spotfes->playlists = (tPlaylists**)malloc(sizeof(tPlaylists*) * 16);
    spotfes->playlists_qty = (int*)malloc(sizeof(int));
    (*spotfes->playlists_qty) = 0;
    (*spotfes->playlists_allocs) = 16;

    for (int m = 0; m < 128; m++) {
        spotfes->artists[m] = AllocateArtists();
        spotfes->tracks[m] = AllocateTracks();

        if (m < 16) {
            spotfes->playlists[m] = AllocatePlaylists();
        }
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
    printf("♬  SPOTFES by M&M  ♬\n\n");
    printf("1 - Pesquisar músicas\n");
    printf("2 - Detalhar uma música\n");
    printf("3 - Criar uma playlist\n");
    printf("4 - Exibir playlists\n");
    printf("5 - Detalhar uma playlist\n");
    printf("6 - Adicionar uma música na playlist\n");
    printf("7 - Recomendar músicas parecidas com uma playlist\n");
    printf("8 - Gerar relatório\n\n");
    printf("♬  Digite a opção desejada: ");
    scanf("%d%*c", &input);

    return input;
}

void SearchTracks(tSpotfes* spotfes) {
    char input[128];
    printf("\n♬  Digite o título de uma música a ser pesquisada: ");
    fgets(input, 128, stdin);
    strcpy(input, strtok(input, "\n"));
    SearchTracksByTitle(input, spotfes->tracks, (*spotfes->tracks_qty));
}

void DetailTrack(tSpotfes* spotfes) {
    int input;
    printf("\n♬  Digite o índice da música a ser detalhada: ");
    scanf("%d", &input);
    SearchTracksByIndex(input, spotfes->tracks, (*spotfes->tracks_qty));
}

void CreatePlaylist(tSpotfes* spotfes) {
    char input[128];
    printf("\n♬  Digite o título de uma playlist a ser criada: ");
    fgets(input, 128, stdin);
    strcpy(input, strtok(input, "\n"));
    spotfes->playlists = NewPlaylist(input, spotfes->playlists, (*spotfes->playlists_qty));

    *spotfes->playlists_qty += 1;

    if ((*spotfes->playlists_qty) > (*spotfes->playlists_allocs)) {
        (*spotfes->playlists_allocs) *= 2;

        spotfes->playlists = ReallocateMorePlaylists(spotfes->playlists, (*spotfes->playlists_allocs));
    }
}

int GetPlaylistQty(tSpotfes* spotfes) {
    return *(spotfes->playlists_qty);
}

void ListPlaylists(tSpotfes* spotfes) {
    DisplayPlaylists(spotfes->playlists, (*spotfes->playlists_qty));
}

void ListPlaylist(tSpotfes* spotfes) {
    int input;
    printf("\n♬  Digite o índice da playlist a ser listada: ");
    scanf("%d", &input);
    SearchPlaylistsByIndex(input, spotfes->playlists, (*spotfes->playlists_qty));
}

void AddTrackToPlaylist(tSpotfes* spotfes) {
    int tracks_index, playlist_index;
    printf("\n♬  Digite o índice da música e da playlist a qual será adicionada: ");
    scanf("%d %d", &tracks_index, &playlist_index);
    LinkTrackToPlaylist(spotfes->playlists[playlist_index], spotfes->tracks[tracks_index]);
}

/*void RecommendSimilarTrack(tSpotfes* spotfes) {
    int playlist_index, qty;
    printf("\n♬  Digite o índice da playlist e a quantidade de músicas similares a serem recomendadas: ");
    scanf("%d %d", &playlist_index, &qty);
    ComparePlaylistToTracks(spotfes, spotfes->playlists[playlist_index], qty);
}

float* GetTracksFeatures(tSpotfes* spotfes, int m) {
    return GetFeatures(spotfes->tracks[m]);
}*/