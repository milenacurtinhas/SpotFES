#include "libraries.h"

struct tplaylists {
    int index;
    char* playlist_name;
    int tracks_number;
    int* tracks_index;
};

tPlaylists* AllocatePlaylists() {
    return (tPlaylists*)malloc(sizeof(tPlaylists));
}

void FreeUpPlaylists(tPlaylists* playlists) {
    free(playlists);
}