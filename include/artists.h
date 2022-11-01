#ifndef TARTISTS_H
#define TARTISTS_H

typedef struct tartists tArtists;

tArtists* AllocateArtists();
void FreeUpArtists(tArtists* artists);
int ReadArtistsDataFiles(tArtists** artists, FILE* artists_data);
tArtists* FindEquivalentArtistByID(tArtists** artists, int artists_qty, char* target_id);

#endif