#ifndef TPLAYLISTS_H
#define TPLAYLISTS_H

typedef struct tplaylists tPlaylists;

tPlaylists* AllocatePlaylists();
void FreeUpPlaylists(tPlaylists* playlists);

#endif