#include "libraries.h"

struct tartists {
    char id[23];
    int followers;
    char** genres;
    int genres_qty;
    char* artist_name;
    int popularity;
};

tArtists* AllocateArtists() {
    return (tArtists*)malloc(sizeof(tArtists));
}

void FreeUpArtists(tArtists* artists) {
    free(artists);
}