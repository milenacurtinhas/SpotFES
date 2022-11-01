#include "libraries.h"

struct ttracks {
    int index;
    char id[23];
    char* track_name;
    int popularity;
    int duration_ms;
    int explicit;
    char** track_artists;
    char* artists_ids[23];
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
        FreeUpArtists(tracks->artists[m]);
    }
    FreeAndNullPointer(tracks->track_artists);
    FreeAndNullPointer(tracks->track_name);
    FreeAndNullPointer(tracks->artists);
    FreeAndNullPointer(tracks);
}

int ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data) {
    char buffer[2048];
    int tracks_qty = 1, line_size, name_size, artists_line_size;

    for (int m = 0; fgets(buffer, 2048, tracks_data) && !EndOfFile(buffer[0]); m++) {
        line_size = strlen(buffer);
        tracks_qty = m + 1;

        tracks[m]->index = m;

        strcpy(tracks[m]->id, strtok(buffer, ";"));

        char name[line_size];
        strcpy(name, strtok(NULL, ";"));
        name_size = strlen(name);
        tracks[m]->track_name = (char*)malloc(sizeof(name_size + 1));
        strcpy(tracks[m]->track_name, name);

        tracks[m]->popularity = atoi(strtok(NULL, ";"));
        tracks[m]->duration_ms = atoi(strtok(NULL, ";"));
        tracks[m]->explicit = atoi(strtok(NULL, ";"));

        char track_artists_line[line_size];
        strcpy(track_artists_line, strtok(NULL, ";"));

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

        artists_line_size = strlen(track_artists_line);
        tracks[m]->artists_qty = GetValueQuantity(track_artists_line, artists_line_size);
        tracks[m]->track_artists = (char**)malloc(sizeof(char*) * tracks[m]->artists_qty);

        for (int mm = 0; mm < tracks[m]->artists_qty; mm++) {
            if (!mm) {
                strcpy(track_artists_line, (strtok(track_artists_line, "|")));
                strcpy(artists_ids_line, (strtok(artists_ids_line, "|")));
            } else if (mm == tracks[m]->artists_qty - 1) {
                strcpy(track_artists_line, (strtok(NULL, "\n")));
                strcpy(artists_ids_line, (strtok(NULL, "\n")));
            } else {
                strcpy(track_artists_line, (strtok(NULL, "|")));
                strcpy(artists_ids_line, (strtok(NULL, "|")));
            }
            artists_line_size = strlen(track_artists_line);
            tracks[m]->track_artists[mm] = (char*)malloc(sizeof(artists_line_size + 1));
            strcpy(tracks[m]->track_artists[mm], track_artists_line);
            strcpy(tracks[m]->artists_ids[mm], artists_ids_line);
        }
    }

    return tracks_qty;
}

void LinkArtistsToTracks(tSpotfes* spotfes, tArtists** artists, tTracks** tracks) {
    int artists_qty = GetArtistsQuantity(spotfes);
    int tracks_qty = GetTracksQuantity(spotfes);
    char target_id[23];

    for (int m = 0; m < tracks_qty; m++) {
        tracks[m]->artists = (tArtists**)malloc(sizeof(tArtists*) * tracks[m]->artists_qty);

        for (int mm = 0; mm < tracks[m]->artists_qty; mm++) {
            strcpy(tracks[m]->artists_ids[mm], target_id);

            tracks[m]->artists[mm] = (tArtists*)malloc(sizeof(tArtists*));
            tracks[m]->artists[mm] = FindEquivalentArtistByID(artists, artists_qty, target_id);
        }
    }
}