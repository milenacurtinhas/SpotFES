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