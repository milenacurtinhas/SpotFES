#ifndef TSPOTFES_H
#define TSPOTFES_H

typedef struct tspotfes tSpotfes;

void CheckDataFilesPath(int argc);
tSpotfes* AllocateSpotfes();
void FreeUpSpotfes(tSpotfes* spotfes);
void ReadSpotifyDataFiles(tSpotfes* spotfes, char** argv);
int GetArtistsQuantity(tSpotfes* spotfes);
int GetTracksQuantity(tSpotfes* spotfes);
int GetPlaylistsQuantity(tSpotfes* spotfes);
int SetUpMainMenu(int input);
void SearchTracks(tSpotfes* spotfes);
void DetailTrack(tSpotfes* spotfes);
void CreatePlaylist(tSpotfes* spotfes);
int GetPlaylistQty(tSpotfes* spotfes);
void ListPlaylists(tSpotfes* spotfes);
void ListPlaylist(tSpotfes* spotfes);
void AddTrackToPlaylist(tSpotfes* spotfes);
void RecommendSimilarTrack(tSpotfes* spotfes);
float* GetTracksFeatures(tSpotfes* spotfes, int m);

#endif