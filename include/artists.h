#ifndef TARTISTS_H
#define TARTISTS_H

typedef struct tartists tArtists;

tArtists* AllocateArtists();
tArtists** ReallocateMoreArtists(tArtists** artists, int new_size);
tArtists** ReallocateLessArtists(tArtists** artists, int old_size, int new_size);
void FreeUpArtists(tArtists* artists);
tArtists** ReadArtistsDataFiles(tArtists** artists, FILE* artists_data, int* artists_qty);
void ReadArtistsGenres(tArtists* artist, char* line);
void PrintTrackArtistsDetails(tArtists** artists, int artists_qty);
char* GetArtistID(tArtists* artist);
void ArtistsAddedToPlaylistCounter (tArtists* artist);
int GetArtistAddedCounter(tArtists* artist);
void PrintArtistName(tArtists* artist);
int GetAddMostAddedArtist (tArtists** artists, int qty);

#endif