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

tTracks** ReallocateMoreTracks(tTracks** tracks, int new_size) {
    tTracks** new = NULL;
    new = (tTracks**)realloc(tracks, sizeof(tTracks*) * new_size);
    tracks = new;

    for (int m = new_size / 2; m < new_size; m++) {
        tracks[m] = AllocateTracks();
    }

    return tracks;
}

tTracks** ReallocateLessTracks(tTracks** tracks, int old_size, int* new_size) {
    for (int m = (*new_size); m < old_size; m++) {
        FreeUpTracks(tracks[m]);
    }

    tTracks** new = NULL;
    new = (tTracks**)realloc(tracks, sizeof(tTracks*) * *new_size);
    tracks = new;

    return tracks;
}

void FreeUpTracks(tTracks* tracks) {
    for (int m = 0; m < tracks->artists_qty; m++) {
        FreeAndNullPointer(tracks->track_artists[m]);
        FreeAndNullPointer(tracks->artists_ids[m]);
    }

    FreeAndNullPointer(tracks->id);
    FreeAndNullPointer(tracks->track_name);
    FreeAndNullPointer(tracks->track_artists);
    FreeAndNullPointer(tracks->artists_ids);
    FreeAndNullPointer(tracks->artists);
    FreeAndNullPointer(tracks);
}

tTracks** ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data, int* tracks_qty) {
    char* buffer = (char*)malloc(sizeof(char) * 2048);
    int alloc_size = 128, line_size, artists_line_size;

    for (int m = 0; fgets(buffer, 2048, tracks_data) && !EndOfFile(buffer[0]); m++) {
        line_size = strlen(buffer);
        *tracks_qty = m + 1;

        if (*tracks_qty > alloc_size) {
            alloc_size *= 2;
            tracks = ReallocateMoreTracks(tracks, alloc_size);
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

    FreeAndNullPointer(buffer);

    if (*tracks_qty < alloc_size) {
        tracks = ReallocateLessTracks(tracks, alloc_size, tracks_qty);
    }

    return tracks;
}

void LinkArtistsToTracks(tSpotfes* spotfes, tTracks** tracks, tArtists** artists) {
    int all_artists_qty = GetArtistsQuantity(spotfes);
    int tracks_qty = GetTracksQuantity(spotfes);
    char all_artists_ids[23];
    // varre todas as tracks
    for (int m = 0; m < tracks_qty; m++) {
        tracks[m]->artists = (tArtists**)malloc(sizeof(tArtists*) * tracks[m]->artists_qty);
        // varre todos os artistas da track
        for (int mm = 0; mm < tracks[m]->artists_qty; mm++) {
            // varre todos os artistas do spotfes
            for (int mmm = 0; mmm < all_artists_qty; mmm++) {
                strcpy(all_artists_ids, GetArtistID(artists[mmm]));
                if (strcmp(all_artists_ids, tracks[m]->artists_ids[mm]) == 0) {
                    tracks[m]->artists[mm] = artists[mmm];
                }
            }
        }
    }
}