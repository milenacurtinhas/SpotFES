#ifndef TPLAYLISTS_H
#define TPLAYLISTS_H

typedef struct tplaylists tPlaylists;

tPlaylists** AllocatePlaylists(int* playlists_qty, int* playlists_allocs);
tPlaylists* AllocatePlaylist();
tPlaylists** ReadBinaryPlaylists(FILE* playlists_file, int* playlists_qty, int* playlists_allocs);
void LinkBinaryTracksToPlaylists(tPlaylists** playlists, int playlists_qty, tTracks** tracks);
void NewPlaylist(char* input, tPlaylists** playlists, int playlists_qty);
tPlaylists** ReallocateMorePlaylists(tPlaylists** playlists, int new_size);
void DisplayPlaylists(tPlaylists** playlists, int playlists_qty);
void SearchPlaylistByIndex(int input, tPlaylists** playlists);
tPlaylists* LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track);
char* GetPlaylistName(tPlaylists* playlist);
int GetPlaylistTracksQuantity(tPlaylists* playlist);
void ComparePlaylistToTracks(tSpotfes* spotfes, tPlaylists* playlist, int qty);
void GetAverages(tPlaylists* playlists);
void WriteBinaryPlaylists(tPlaylists** playlists, int quantity);
void FreeUpPlaylists(tPlaylists* playlists);

#endif