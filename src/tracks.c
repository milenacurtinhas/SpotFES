#include "libraries.h"

struct ttracks {
    int index;
    char* id;
    char* track_name;
    int popularity;
    int duration_ms;
    int explicit;
    char** track_artists;
    char** artists_ids;
    int artists_qty;
    int release_year;
    int release_month;
    int release_day;
    float danceability;
    float energy;
    int key;
    float loudness;
    int mode;
    float speechiness;
    float acousticness;
    float instrumentalness;
    float liveness;
    float valence;
    float tempo;
    int time_signature;
    tArtists** artists;
};

tTracks* AllocateTracks() {
    return (tTracks*)malloc(sizeof(tTracks));
}

void FreeUpTracks(tTracks* tracks) {
    for (int m = 0; m < tracks->artists_qty; m++) {
        FreeAndNullPointer(tracks->track_artists[m]);
        FreeAndNullPointer(tracks->artists_ids[m]);
        // FreeUpArtists(tracks->artists[m]); // ativar quando fizer a linkagem dos artistas das musicas para as musicas
    }
    FreeAndNullPointer(tracks->id);
    FreeAndNullPointer(tracks->track_name);
    FreeAndNullPointer(tracks->track_artists);
    FreeAndNullPointer(tracks->artists_ids);
    // FreeAndNullPointer(tracks->artists); // ativar quando fizer a linkagem dos artistas das musicas para as musicas
    FreeAndNullPointer(tracks);
}

int ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data) {
    char* buffer = (char*)malloc(sizeof(char) * 2048);
    int alloc_size = 128, tracks_qty = 1, line_size, artists_line_size;

    for (int m = 0; fgets(buffer, 2048, tracks_data) && !EndOfFile(buffer[0]); m++) {
        line_size = strlen(buffer);
        tracks_qty = m + 1;

        if (tracks_qty > alloc_size) {
            alloc_size *= 2;
            // realloc;
        }

        tracks[m]->index = m;
        tracks[m]->id = strdup(strtok(buffer, ";"));
        tracks[m]->track_name = strdup(strtok(NULL, ";"));
        tracks[m]->popularity = atoi(strtok(NULL, ";"));
        tracks[m]->duration_ms = atoi(strtok(NULL, ";"));
        tracks[m]->explicit = atoi(strtok(NULL, ";"));

        char artists_line[line_size];
        strcpy(artists_line, strtok(NULL, ";"));

        char artists_ids_line[line_size];
        strcpy(artists_ids_line, strtok(NULL, ";"));

        tracks[m]->release_year = atoi(strtok(NULL, "-"));
        tracks[m]->release_month = atoi(strtok(NULL, "-"));
        tracks[m]->release_day = atoi(strtok(NULL, ";"));
        tracks[m]->danceability = atof(strtok(NULL, ";"));
        tracks[m]->energy = atof(strtok(NULL, ";"));
        tracks[m]->key = atoi(strtok(NULL, ";"));
        tracks[m]->loudness = atof(strtok(NULL, ";"));
        tracks[m]->mode = atoi(strtok(NULL, ";"));
        tracks[m]->speechiness = atof(strtok(NULL, ";"));
        tracks[m]->acousticness = atof(strtok(NULL, ";"));
        tracks[m]->instrumentalness = atof(strtok(NULL, ";"));
        tracks[m]->liveness = atof(strtok(NULL, ";"));
        tracks[m]->valence = atof(strtok(NULL, ";"));
        tracks[m]->tempo = atof(strtok(NULL, ";"));
        tracks[m]->time_signature = atoi(strtok(NULL, "\n"));

        artists_line_size = strlen(artists_line);
        tracks[m]->artists_qty = GetValueQuantity(artists_line, artists_line_size);
        tracks[m]->track_artists = (char**)malloc(sizeof(char*) * tracks[m]->artists_qty);
        tracks[m]->artists_ids = (char**)malloc(sizeof(char*) * tracks[m]->artists_qty);

        for (int mm = 0; mm < tracks[m]->artists_qty; mm++) {
            if (!mm) {
                tracks[m]->track_artists[mm] = strdup(strtok(artists_line, "|"));
            } else if (mm == tracks[m]->artists_qty - 1) {
                tracks[m]->track_artists[mm] = strdup(strtok(NULL, "\n"));
            } else {
                tracks[m]->track_artists[mm] = strdup(strtok(NULL, "|"));
            }
        }

        for (int mm = 0; mm < tracks[m]->artists_qty; mm++) {
            if (!mm) {
                tracks[m]->artists_ids[mm] = strdup(strtok(artists_ids_line, "|"));
            } else if (mm == tracks[m]->artists_qty - 1) {
                tracks[m]->artists_ids[mm] = strdup(strtok(NULL, "\n"));
            } else {
                tracks[m]->artists_ids[mm] = strdup(strtok(NULL, "|"));
            }
        }
    }

    if (tracks_qty < alloc_size) {
        // realloc;
    }

    FreeAndNullPointer(buffer);

    return tracks_qty;
}