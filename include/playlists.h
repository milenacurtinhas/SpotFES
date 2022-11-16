#ifndef TPLAYLISTS_H
#define TPLAYLISTS_H

typedef struct tplaylists tPlaylists;

tPlaylists** AllocatePlaylists(int* playlists_qty, int* playlists_allocs);
tPlaylists* AllocatePlaylist();
tPlaylists** ReallocateMorePlaylists(tPlaylists** playlists, int new_size);
void FreeUpPlaylists(tPlaylists* playlists);
void NewPlaylist(char* input, tPlaylists** playlists, int playlists_qty);
void DisplayPlaylists(tPlaylists** playlists, int playlists_qty);
void SearchPlaylistByIndex(int input, tPlaylists** playlists);
char* GetPlaylistName(tPlaylists* playlist);
tPlaylists* LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track);
void ComparePlaylistToTracks(tSpotfes* spotfes, tPlaylists* playlist, int qty);
void GetAverages(tPlaylists* playlists);
int GetPlaylistTracksQuantity(tPlaylists* playlist);
void WriteBinaryPlaylists(tPlaylists** playlists, int quantity);
tPlaylists** ReadBinaryPlaylists(FILE* playlists_file, int* playlists_qty, int* playlists_allocs);
void LinkBinaryTracksToPlaylists(tPlaylists** playlists, int playlists_qty, tTracks** tracks);

#endif