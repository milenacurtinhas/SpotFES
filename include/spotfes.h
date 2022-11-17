#ifndef TSPOTFES_H
#define TSPOTFES_H

typedef struct tspotfes tSpotfes;

tSpotfes* ReadSpotfesDataFiles(tSpotfes* spotfes, char** argv);
tSpotfes* AllocateSpotfes(tSpotfes* spotfes);
int GetArtistsQuantity(tSpotfes* spotfes);
int GetTracksQuantity(tSpotfes* spotfes);

int SetUpMainMenu();
void SearchTracks(tSpotfes* spotfes);
void DetailTrack(tSpotfes* spotfes);
void CreatePlaylist(tSpotfes* spotfes);
void ListPlaylists(tSpotfes* spotfes);
void DetailPlaylist(tSpotfes* spotfes);
void AddTrackToPlaylist(tSpotfes* spotfes);
void RecommendSimilarTrack(tSpotfes* spotfes);
float* GetTracksFeatures(tSpotfes* spotfes, int m);
void RelatesDistanceToTrack(tSpotfes* spotfes, float* euclidean_distance);
float GetTrackDistance(tSpotfes* spotfes, int index);
void PrintSimilarTrack(tSpotfes* spotfes, int index);
void GenerateReport(tSpotfes* spotfes);
void QuitProgram(tSpotfes* spotfes);
void FreeUpSpotfes(tSpotfes* spotfes);

#endif