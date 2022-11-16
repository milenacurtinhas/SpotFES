#include "libraries.h"

struct tplaylists {
    int index;
    int name_size;
    char* playlist_name;
    int tracks_qty;
    int* tracks_index;
    tTracks** tracks;
    float* averages;
    int* tracks_alloc_size;
};

tPlaylists** AllocatePlaylists(int* playlists_qty, int* playlists_allocs) {
    FILE* playlists_file = fopen("bin/playlists.bin", "rb");

    if (playlists_file == NULL) {
        tPlaylists** playlists = (tPlaylists**)calloc(sizeof(tPlaylists*), STARTER_PLAYLISTS_SIZE);

        for (int m = 0; m < STARTER_PLAYLISTS_SIZE; m++) {
            playlists[m] = AllocatePlaylist();
        }

        return playlists;
    } else {
        return ReadBinaryPlaylists(playlists_file, playlists_qty, playlists_allocs);
    }
}

tPlaylists* AllocatePlaylist() {
    tPlaylists* playlist = (tPlaylists*)calloc(sizeof(tPlaylists), 1);
    playlist->playlist_name = (char*)calloc(sizeof(char), STRING_BUFFER_SIZE);
    playlist->tracks_index = (int*)calloc(sizeof(int), STARTER_PLAYLISTS_SIZE);
    playlist->tracks = (tTracks**)calloc(sizeof(tTracks*), STARTER_PLAYLISTS_SIZE);
    playlist->averages = (float*)calloc(sizeof(float), TRACK_STATS_QUANTITY);
    playlist->tracks_alloc_size = (int*)calloc(sizeof(int), 1);
    *playlist->tracks_alloc_size = STARTER_PLAYLISTS_SIZE;

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
    FreeAndNullPointer(playlists->tracks_index);
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
        RED_COLOUR;
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
        NORMAL_COLOUR;
    } else {
        printf("• Informações sobre as playlists:\n\n");
        NORMAL_COLOUR;
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

tPlaylists* LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track) {
    if (playlist->tracks_qty == *playlist->tracks_alloc_size) {
        *playlist->tracks_alloc_size *= 2;

        int* new_index = NULL;
        new_index = (int*)realloc(playlist->tracks_index, sizeof(int) * *playlist->tracks_alloc_size);
        playlist->tracks_index = new_index;
        playlist->tracks = ReallocateMorePlaylistsTracks(playlist->tracks, *playlist->tracks_alloc_size);
    }

    playlist->tracks_index[playlist->tracks_qty] = GetTrackIndex(track);
    playlist->tracks[playlist->tracks_qty] = track;

    playlist->tracks_qty++;

    TrackAddedToPlaylistCounter(track);

    return playlist;
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
    for (int m = 0; m < TRACK_STATS_QUANTITY; m++) {
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
        fwrite(playlists[m]->tracks_index, sizeof(int), playlists[m]->tracks_qty, playlists_file);
        fwrite(playlists[m]->averages, sizeof(float), TRACK_STATS_QUANTITY, playlists_file);
        fwrite(playlists[m]->tracks_alloc_size, sizeof(int), 1, playlists_file);
    }

    fclose(playlists_file);
}

tPlaylists** ReadBinaryPlaylists(FILE* playlists_file, int* playlists_qty, int* playlists_allocs) {
    size_t read = fread(playlists_qty, sizeof(int), 1, playlists_file);

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
        read += fread(&playlists[m]->index, sizeof(int), 1, playlists_file);
        read += fread(&playlists[m]->name_size, sizeof(int), 1, playlists_file);
        read += fread(playlists[m]->playlist_name, sizeof(char), playlists[m]->name_size, playlists_file);
        read += fread(&playlists[m]->tracks_qty, sizeof(int), 1, playlists_file);

        if (playlists[m]->tracks_qty > *playlists[m]->tracks_alloc_size) {
            while (playlists[m]->tracks_qty > *playlists[m]->tracks_alloc_size) {
                *playlists[m]->tracks_alloc_size *= 2;
            }

            tTracks** new_tracks = NULL;
            int* new_index = NULL;

            new_index = (int*)realloc(playlists[m]->tracks_index, sizeof(int) * *playlists[m]->tracks_alloc_size);
            new_tracks = (tTracks**)realloc(playlists[m]->tracks, sizeof(tTracks*) * *playlists[m]->tracks_alloc_size);

            playlists[m]->tracks_index = new_index;
            playlists[m]->tracks = new_tracks;
        }

        read += fread(playlists[m]->tracks_index, sizeof(int), playlists[m]->tracks_qty, playlists_file);
        read += fread(playlists[m]->averages, sizeof(float), TRACK_STATS_QUANTITY, playlists_file);
        read += fread(playlists[m]->tracks_alloc_size, sizeof(int), 1, playlists_file);
    }

    if (read) {
        fclose(playlists_file);
        return playlists;
    } else {
        RED_COLOUR;
        printf("• ERRO: Leitura incompleta dos arquivos binários das playlists.\n\n");
        NORMAL_COLOUR;
        exit(1);
    }
}

void LinkBinaryTracksToPlaylists(tPlaylists** playlists, int playlists_qty, tTracks** tracks) {
    for (int m = 0; m < playlists_qty; m++) {
        for (int mm = 0; mm < playlists[m]->tracks_qty; mm++) {
            playlists[m]->tracks[mm] = tracks[playlists[m]->tracks_index[mm]];
        }
    }
}