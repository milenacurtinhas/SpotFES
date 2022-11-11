#include "libraries.h"

struct tplaylists {
    int index;
    char* playlist_name;
    int tracks_qty;
    // int* tracks_index;
    tTracks** tracks;
    float* averages;
    int* tracks_alloc_size;
};

tPlaylists* AllocatePlaylists() {
    return (tPlaylists*)malloc(sizeof(tPlaylists));
}

tPlaylists** ReallocateMorePlaylists(tPlaylists** playlists, int new_size) {
    tPlaylists** new = NULL;
    new = (tPlaylists**)realloc(playlists, sizeof(tPlaylists*) * new_size);
    playlists = new;

    for (int m = new_size / 2; m < new_size; m++) {
        playlists[m] = AllocatePlaylists();
    }

    return playlists;
}

void FreeUpPlaylists(tPlaylists* playlists) {
    FreeAndNullPointer(playlists->tracks_alloc_size);
    FreeAndNullPointer(playlists->tracks);
    FreeAndNullPointer(playlists->playlist_name);
    FreeAndNullPointer(playlists->averages);
    FreeAndNullPointer(playlists);
}

void NewPlaylist(char* input, tPlaylists** playlists, int playlists_qty) {
    playlists[playlists_qty]->index = playlists_qty;
    playlists[playlists_qty]->playlist_name = strdup(input);
    playlists[playlists_qty]->tracks = (tTracks**)malloc(sizeof(tTracks*) * 16);
    playlists[playlists_qty]->averages = (float*)malloc(sizeof(float) * 8);
    playlists[playlists_qty]->tracks_alloc_size = (int*)malloc(sizeof(int));
    (*playlists[playlists_qty]->tracks_alloc_size) = 16;
}

void DisplayPlaylists(tPlaylists** playlists, int playlists_qty) {
    printf("\n♪  Informações sobre as playlists  ♪\n");
    for (int m = 0; m < playlists_qty; m++) {
        printf("Nome: %s - Índice: %d - Quantidade de músicas: %d\n", playlists[m]->playlist_name, playlists[m]->index, playlists[m]->tracks_qty);
    }
    printf("\n");
}

void SearchPlaylistsByIndex(int input, tPlaylists** playlists) {
    printf("\n♪  Informações sobre a playlist  ♪\n");
    printf("Nome: %s\n", playlists[input]->playlist_name);
    DisplayTracks(playlists[input]->tracks, playlists[input]->tracks_qty);
}

void LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track) {
    if (playlist->tracks_qty == (*playlist->tracks_alloc_size)) {  // ESSE TAMBEM TEM QUE SER IGUAL ASSIM COMO A LINHA 161 DA SPOTFES.C
        (*playlist->tracks_alloc_size) *= 2;

        // LEMBRAR DE FAZER REALLOC DAS TARCKS DAS PLAYLISTS
        // ReallocateMorePlaylistsTracks(playlist->tracks, playlist->tracks_alloc_size);
    }

    playlist->tracks[playlist->tracks_qty] = track;

    playlist->tracks_qty += 1;
}

void ComparePlaylistToTracks(tSpotfes* spotfes, tPlaylists* playlist, int qty) {
    GetAverages(playlist);

    for (int i = 0; i < 8; i++) {
        printf ("%.4f\n", playlist->averages[i]);
    }

    int tracks_qty = GetTracksQuantity(spotfes);
    float EuclideanDistance[tracks_qty];

    for (int m = 0; m < tracks_qty; m++) {
        EuclideanDistance[m] = CalculateEuclideanDistance(GetTracksFeatures(spotfes, m), playlist->averages);
    }

    for (int m = 0; m < 20; m++){
        printf ("%.4f ", EuclideanDistance[m]);
    }

    /*int i, j, aux;
    for (i = 1; i < tracks_qty; i++) {
        for (j = 0; j < tracks_qty - 1; j++) {
            if (EuclideanDistance[j] > EuclideanDistance[j + 1]) {
                aux = EuclideanDistance[j];
                EuclideanDistance[j] = EuclideanDistance[j + 1];
                EuclideanDistance[j + 1] = aux;
            }
        }
    }

    printf("\n♪  Músicas similares a playlist  ♪\n");

    for (int mm = 0; mm < qty; mm++) {
        printf(EuclideanDistance[mm]);
    }*/
}

void GetAverages(tPlaylists* playlist) {
    for (int m = 0; m < 8; m++) {
        playlist->averages[m] = CalculateAverages(m, playlist->tracks, playlist->tracks_qty);
    }
}