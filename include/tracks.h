#ifndef TTRACKS_H
#define TTRACKS_H

typedef struct ttracks tTracks;

tTracks* AllocateTracks();
tTracks** ReallocateMoreTracks(tTracks** tracks, int new_size);
tTracks** ReallocateLessTracks(tTracks** tracks, int old_size, int* new_size);
tTracks** ReallocateMorePlaylistsTracks (tTracks** tracks, int new_size);
void FreeUpTracks(tTracks* tracks);
tTracks** ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data, int* tracks_qty);
void LinkArtistsToTracks(tSpotfes* spotfes, tTracks** tracks, tArtists** artists);
void SearchTracksByTitle(char* input, tTracks** tracks, int tracks_qty);
void SearchTracksByIndex(int input, tTracks** tracks);
void OpenTrack(tTracks* track);
void DisplayTracks(tTracks** tracks_from_playlist, int tracks_qty);
void PutFeaturesInArray(tTracks* track);
float CalculateAverages(int feature, tTracks** tracks, int tracks_qty);
float GetFeatureValue(tTracks* track, int feature);
float* GetFeatures(tTracks* tracks);
//void SaveEuclideanDistanceToTrack(tTracks* track, float euclidean_distance);

#endif