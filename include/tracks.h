#ifndef TTRACKS_H
#define TTRACKS_H

typedef struct ttracks tTracks;

tTracks* AllocateTracks();
tTracks** ReallocateMoreTracks(tTracks** tracks, int new_size);
tTracks** ReallocateLessTracks(tTracks** tracks, int old_size, int new_size);
tTracks** ReallocateMorePlaylistsTracks(tTracks** tracks, int new_size);
void FreeUpTracks(tTracks* tracks);
tTracks** ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data, int* tracks_qty);
void ReadTrackReleaseDate(tTracks* track, char* line);
void ReadTrackArtists(tTracks* track, char* line);
void ReadTrackArtistsIDs(tTracks* track, char* line);
void PutFeaturesInArray(tTracks* track);
void LinkArtistsToTracks(tSpotfes* spotfes, tTracks** tracks, tArtists** artists);
void PrintTrackArtists(tTracks* track, int quantity);
void PrintShortTracksDetails(tTracks* track);
void SearchTracksByTitle(char* input, tTracks** tracks, int tracks_qty);
void SearchTracksByIndex(int input, tTracks** tracks);
void OpenTrack(tTracks* track);
char* GetTrackName(tTracks* track);
void ShowPlaylistTracks(tTracks** tracks_from_playlist, int tracks_qty);
float CalculateAverages(int feature, tTracks** tracks, int tracks_qty);
float GetFeatureValue(tTracks* track, int feature);
int GetTrackArtistsQuantity(tTracks* track);
float* GetFeatures(tTracks* tracks);
void SaveEuclideanDistanceToTrack(tTracks* track, float euclidean_distance);
float GetDistance(tTracks* tracks);
void TrackAddedToPlaylistCounter(tTracks* track);
int GetTracksAddedCounter(tTracks* track);
void PrintTrackName(FILE* tracks_file, tTracks* track);
int GetAddMostAddedTrack(tTracks** tracks, int qty);
void WriteBinaryIndex(FILE* file, tTracks** tracks, int quantity);

#endif