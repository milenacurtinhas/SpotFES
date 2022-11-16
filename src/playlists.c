#include "libraries.h"

struct tplaylists {
    int index;
    int name_size;
    char* playlist_name;
    int tracks_qty;
    tTracks** tracks;
    float* averages;
    int* tracks_alloc_size;
};

tPlaylists** AllocatePlaylists(int* playlists_qty, int* playlists_allocs) {
    FILE* playlists_file = fopen("bin/playlists.bin", "rb");
    if (playlists_file == NULL) {
        tPlaylists** playlists = (tPlaylists**)calloc(sizeof(tPlaylists*), 16);

        for (int m = 0; m < 16; m++) {
            playlists[m] = AllocatePlaylist();
        }

        return playlists;
    } else {
        return ReadBinaryPlaylists(playlists_file, playlists_qty, playlists_allocs);
    }
}

tPlaylists* AllocatePlaylist() {
    tPlaylists* playlist = (tPlaylists*)calloc(sizeof(tPlaylists), 1);

    playlist->playlist_name = (char*)calloc(sizeof(char), 64);
    playlist->tracks = (tTracks**)calloc(sizeof(tTracks*), 16);
    playlist->averages = (float*)calloc(sizeof(float), 8);
    playlist->tracks_alloc_size = (int*)calloc(sizeof(int), 1);
    *playlist->tracks_alloc_size = 16;

    return playlist;
}

tPlaylists** ReallocateMorePlaylists(tPlaylists** playlists, int new_size) {
    tPlaylists** new = NULL;
    new = (tPlaylists**)realloc(playlists, sizeof(tPlaylists*) * new_size);
    playlists = new;

    for (int m = new_size / 2; m < new_size; m++) {
        playlists[m] = AllocatePlaylist();
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
    strcpy(playlists[playlists_qty]->playlist_name, input);
    playlists[playlists_qty]->name_size = strlen(input);
}

void DisplayPlaylists(tPlaylists** playlists, int playlists_qty) {
    if (!playlists_qty) {
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
    } else {
        printf("• Informações sobre as playlists:\n\n");
        for (int m = 0; m < playlists_qty; m++) {
            printf("Nome: %s\n", playlists[m]->playlist_name);
            printf("Índice: %d\n", playlists[m]->index);
            printf("Músicas: %d\n\n", playlists[m]->tracks_qty);
        }
    }
}

void SearchPlaylistByIndex(int input, tPlaylists** playlists) {
    printf("\nNome: %s\n", playlists[input]->playlist_name);
    ShowPlaylistTracks(playlists[input]->tracks, playlists[input]->tracks_qty);
}

char* GetPlaylistName(tPlaylists* playlist) {
    return playlist->playlist_name;
}

void LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track) {
    if (playlist->tracks_qty == *playlist->tracks_alloc_size) {  // ESSE TAMBEM TEM QUE SER IGUAL ASSIM COMO A LINHA 161 DA SPOTFES.C
        *playlist->tracks_alloc_size *= 2;

        playlist->tracks = ReallocateMorePlaylistsTracks(playlist->tracks, *playlist->tracks_alloc_size);
    }

    playlist->tracks[playlist->tracks_qty] = track;

    playlist->tracks_qty++;

    TrackAddedToPlaylistCounter(track);
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

void WriteBinaryPlaylists(tPlaylists** playlists, int quantity) {
    FILE* playlists_file = fopen("bin/playlists.bin", "wb");

    fwrite(&quantity, sizeof(int), 1, playlists_file);

    for (int m = 0; m < quantity; m++) {
        fwrite(&playlists[m]->index, sizeof(int), 1, playlists_file);
        fwrite(&playlists[m]->name_size, sizeof(int), 1, playlists_file);
        fwrite(playlists[m]->playlist_name, sizeof(char), playlists[m]->name_size, playlists_file);
        fwrite(&playlists[m]->tracks_qty, sizeof(int), 1, playlists_file);
        WriteBinaryIndex(playlists_file, playlists[m]->tracks, playlists[m]->tracks_qty);
        fwrite(playlists[m]->averages, sizeof(float), 8, playlists_file);
        fwrite(playlists[m]->tracks_alloc_size, sizeof(int), 1, playlists_file);
    }

    fclose(playlists_file);
}

tPlaylists** ReadBinaryPlaylists(FILE* playlists_file, int* playlists_qty, int* playlists_allocs) {
    fread(playlists_qty, sizeof(int), 1, playlists_file);

    if (*playlists_qty > *playlists_allocs) {
        while (*playlists_qty > *playlists_allocs) {
            *playlists_allocs *= 2;
        }
    }

    tPlaylists** playlists = (tPlaylists**)calloc(sizeof(tPlaylists*), *playlists_allocs);

    for (int m = 0; m < *playlists_allocs; m++) {
        playlists[m] = AllocatePlaylist();
    }

    for (int m = 0; m < *playlists_qty; m++) {
        fread(&playlists[m]->index, sizeof(int), 1, playlists_file);
        fread(&playlists[m]->name_size, sizeof(int), 1, playlists_file);
        fread(playlists[m]->playlist_name, sizeof(char), playlists[m]->name_size, playlists_file);
        fread(&playlists[m]->tracks_qty, sizeof(int), 1, playlists_file);
        ReadBinaryIndex(playlists_file, playlists[m]->tracks, playlists[m]->tracks_qty);
        fread(playlists[m]->averages, sizeof(float), 8, playlists_file);
        fread(playlists[m]->tracks_alloc_size, sizeof(int), 1, playlists_file);
    }

    fclose(playlists_file);

    return playlists;
}