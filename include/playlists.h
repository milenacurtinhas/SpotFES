#ifndef TPLAYLISTS_H
#define TPLAYLISTS_H

typedef struct tplaylists tPlaylists;

tPlaylists* AllocatePlaylist();
tPlaylists** ReallocateMorePlaylists(tPlaylists** playlists, int new_size);
void FreeUpPlaylists(tPlaylists* playlists);
void NewPlaylist(char* input, tPlaylists** playlists, int playlists_qty);
void DisplayPlaylists(tPlaylists** playlists, int playlists_qty);
void SearchPlaylistByIndex(int input, tPlaylists** playlists);
char* GetPlaylistName(tPlaylists* playlist);
void LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track);
void ComparePlaylistToTracks(tSpotfes* spotfes, tPlaylists* playlist, int qty);
void GetAverages(tPlaylists* playlists);
int GetPlaylistTracksQuantity(tPlaylists* playlist);

#endif