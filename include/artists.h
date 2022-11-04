#ifndef TARTISTS_H
#define TARTISTS_H

typedef struct tartists tArtists;

tArtists* AllocateArtists();
tArtists** ReallocateMoreArtists(tArtists** tracks, int new_size);
tArtists** ReallocateLessArtists(tArtists** tracks, int old_size, int* new_size);
void FreeUpArtists(tArtists* artists);
tArtists** ReadArtistsDataFiles(tArtists** artists, FILE* artists_data, int* artists_qty);
char* GetArtistID(tArtists* artist);

#endif