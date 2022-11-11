#ifndef TPLAYLISTS_H
#define TPLAYLISTS_H

typedef struct tplaylists tPlaylists;

tPlaylists* AllocatePlaylists();
void FreeUpPlaylists(tPlaylists* playlists);
void NewPlaylist(char* input, tPlaylists** playlists, int playlists_qty);
void DisplayPlaylists(tPlaylists** palylists, int playlists_qty);
void SearchPlaylistsByIndex(int input, tPlaylists** playlists);
void LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track);
void ComparePlaylistToTracks (tSpotfes* spotfes, tPlaylists* playlist, int qty);
void GetAverages (tPlaylists* playlists);
// tPlaylists* GetPlaylist (tSpotfes* spotfes, int track_index);

#endif