#ifndef TSPOTFES_H
#define TSPOTFES_H

typedef struct tspotfes tSpotfes;

tSpotfes* AllocateSpotfes(tSpotfes* spotfes);
void FreeUpSpotfes(tSpotfes* spotfes);
tSpotfes* ReadSpotifyDataFiles(tSpotfes* spotfes, char** argv);
int SetUpMainMenu();
void QuitProgram(tSpotfes* spotfes);
void SearchTracks(tSpotfes* spotfes);
void DetailTrack(tSpotfes* spotfes);
void CreatePlaylist(tSpotfes* spotfes);
void ListPlaylists(tSpotfes* spotfes);
void DetailPlaylist(tSpotfes* spotfes);
void AddTrackToPlaylist(tSpotfes* spotfes);
void RecommendSimilarTrack(tSpotfes* spotfes);
int GetArtistsQuantity(tSpotfes* spotfes);
int GetTracksQuantity(tSpotfes* spotfes);
int GetPlaylistAddedTracksQuantity(tSpotfes* spotfes, int index);
float* GetTracksFeatures(tSpotfes* spotfes, int m);
void RelatesDistanceToTrack(tSpotfes* spotfes, float* euclidean_distance);
float GetTrackDistance(tSpotfes* spotfes, int index);
void PrintSimilarTrack(tSpotfes* spotfes, int index);

#endif