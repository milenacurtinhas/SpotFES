#include "libraries.h"

struct tplaylists {
    int index;
    char* playlist_name;
    int tracks_qty;
    tTracks** tracks;
    float* averages;
    int* tracks_alloc_size;
};

tPlaylists* AllocatePlaylists() {
    return (tPlaylists*)calloc(sizeof(tPlaylists), 1);
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
    *playlists[playlists_qty]->tracks_alloc_size = 16;
}

void DisplayPlaylists(tPlaylists** playlists, int playlists_qty) {
    if (!playlists_qty) {
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n");
    } else {
        printf("• Informações sobre as playlists:\n\n");
        for (int m = 0; m < playlists_qty; m++) {
            printf("Nome: %s\n", playlists[m]->playlist_name);
            printf("Índice: %d\n", playlists[m]->index);
            printf("Músicas: %d\n", playlists[m]->tracks_qty);
        }
    }
    printf("\n");
}

void SearchPlaylistByIndex(int input, tPlaylists** playlists) {
    printf("\nNome: %s\n", playlists[input]->playlist_name);
    ShowPlaylistTracks(playlists[input]->tracks, playlists[input]->tracks_qty);
}

void LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track) {
    if (playlist->tracks_qty == *playlist->tracks_alloc_size) {  // ESSE TAMBEM TEM QUE SER IGUAL ASSIM COMO A LINHA 161 DA SPOTFES.C
        *playlist->tracks_alloc_size *= 2;

        playlist->tracks = ReallocateMorePlaylistsTracks(playlist->tracks, *playlist->tracks_alloc_size);
    }

    playlist->tracks[playlist->tracks_qty] = track;

    playlist->tracks_qty++;
}

void ComparePlaylistToTracks(tSpotfes* spotfes, tPlaylists* playlist, int qty) {
    int tracks_qty = GetTracksQuantity(spotfes);
    float aux, euclidean_distance[tracks_qty];

    GetAverages(playlist);

    for (int m = 0; m < tracks_qty; m++) {
        euclidean_distance[m] = CalculateEuclideanDistance(GetTracksFeatures(spotfes, m), playlist->averages);
    }

    RelatesDistanceToTrack(spotfes, euclidean_distance);

    for (int m = 0; m < tracks_qty; m++) {
        for (int mm = m; mm < tracks_qty; mm++) {
            if (euclidean_distance[m] > euclidean_distance[mm]) {
                aux = euclidean_distance[m];
                euclidean_distance[m] = euclidean_distance[mm];
                euclidean_distance[mm] = aux;
            }
        }
    }

    for (int m = 0; m < qty; m++) {
        for (int mm = 0; mm < tracks_qty; mm++) {
            float distance = GetTrackDistance(spotfes, mm);

            if (distance == euclidean_distance[m]) {
                if (!m) {
                    printf("\n");
                }
                PrintSimilarTrack(spotfes, mm);
                break;
            }
        }
    }
}

void GetAverages(tPlaylists* playlist) {
    for (int m = 0; m < 8; m++) {
        playlist->averages[m] = CalculateAverages(m, playlist->tracks, playlist->tracks_qty);
    }
}

int GetPlaylistTracksQuantity(tPlaylists* playlist) {
    return playlist->tracks_qty;
}