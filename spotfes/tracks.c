#include "libraries.h"

struct ttracks {
    int index;
    char id[22];
    char* track_name;
    int popularity;
    int duration_ms;
    int explicit;
    char** track_artists;
    char* artists_ids[22];
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
    free(tracks);
}