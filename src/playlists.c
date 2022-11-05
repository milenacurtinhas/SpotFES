#include "libraries.h"

struct tplaylists {
    int index;
    char* playlist_name;
    int tracks_qty;
    int* tracks_index;
};

tPlaylists* AllocatePlaylists() {
    return (tPlaylists*)malloc(sizeof(tPlaylists));
}

void FreeUpPlaylists(tPlaylists* playlists) {
    FreeAndNullPointer(playlists->playlist_name);
    FreeAndNullPointer(playlists->tracks_index);
    FreeAndNullPointer(playlists);
}