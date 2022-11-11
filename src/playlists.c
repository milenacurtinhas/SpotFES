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
    FreeAndNullPointer(playlists->playlist_name);
    FreeAndNullPointer(playlists->averages);
    // FreeAndNullPointer(playlists->tracks_index);
    FreeAndNullPointer(playlists);
}

tPlaylists** NewPlaylist(char* input, tPlaylists** playlists, int playlists_qty) {
    playlists[playlists_qty]->index = playlists_qty;
    playlists[playlists_qty]->playlist_name = input;
    playlists[playlists_qty]->tracks = (tTracks**)malloc(sizeof(tTracks*) * 16);
    *(playlists[playlists_qty]->tracks_alloc_size) = 16;

    return playlists;
}

void DisplayPlaylists(tPlaylists** playlists, int playlists_qty) {
    for (int m = 0; m < playlists_qty; m++) {
        printf("\n♪  Informações sobre as playlists  ♪\n");
        printf("Índice da playlist: %d, nome da playlist: %s, quantidade de músicas: %d\n", playlists[m]->index, playlists[m]->playlist_name, playlists[m]->tracks_qty);
    }
}
void SearchPlaylistsByIndex(int input, tPlaylists** playlists, int playlists_qty) {
    for (int m = 0; m < playlists_qty; m++) {
        if (input == playlists[m]->index) {
            printf("\n♪  Informações sobre a playlist  ♪\n");
            printf("Playlist: %s\n", playlists[m]->playlist_name);
            printf("Músicas: ");
            DisplayTracks(playlists[m]->tracks, playlists[m]->tracks_qty);
            break;
        }
    }
}

void LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track) {
    playlist->tracks[playlist->tracks_qty] = track;

    playlist->tracks_qty += 1;

    if (playlist->tracks_qty > *(playlist->tracks_alloc_size)) {
        *(playlist->tracks_alloc_size) *= 2;

        //LEMBRAR DE FAZER REALLOC DAS TARCKS DAS PLAYLISTS
        //ReallocateMorePlaylistsTracks(playlist->tracks, playlist->tracks_alloc_size);
    }
}

/*void GetAverages(tPlaylists* playlist) {
    playlist->averages = (float*)malloc(sizeof(float) * 8);

    for (int m = 0; m < 8; m++) {
        playlist->averages[m] = CalculateAverages(m, playlist->tracks, playlist->tracks_qty);
    }
}

void ComparePlaylistToTracks(tSpotfes* spotfes, tPlaylists* playlist, int qty) {
    GetAverages(playlist);

    int tracks_qty = GetTracksQuantity(spotfes);
    float EuclideanDistance[tracks_qty];

    for (int m = 0; m < tracks_qty; m++) {
        EuclideanDistance[m] = CalculateEuclideanDistance(GetTracksFeatures(spotfes, m), playlist->averages);
    }

    int i, j, aux;
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
    }
}*/