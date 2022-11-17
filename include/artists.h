#ifndef TARTISTS_H
#define TARTISTS_H

typedef struct tartists tArtists;

tArtists* AllocateArtists();
tArtists** ReadArtistsDataFiles(tArtists** artists, FILE* artists_data, int* artists_qty);
void ReadArtistsGenres(tArtists* artist, char* line);
tArtists** ReallocateMoreArtists(tArtists** artists, int new_size);
tArtists** ReallocateLessArtists(tArtists** artists, int old_size, int new_size);
char* GetArtistID(tArtists* artist);
void ReadBinaryArtists(tArtists** artists, int quantity);
void PrintTrackArtistsDetails(tArtists** artists, int artists_qty);
void ArtistsAddedToPlaylistCounter(tArtists* artist);
int GetAddMostAddedArtist(tArtists** artists, int qty);
int GetArtistAddedCounter(tArtists* artist);
void PrintArtistName(FILE* artists_file, tArtists* artist);
void WriteBinaryArtists(tArtists** artists, int quantity);
void FreeUpArtists(tArtists* artists);

#endif