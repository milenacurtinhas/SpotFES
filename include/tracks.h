#ifndef TTRACKS_H
#define TTRACKS_H

typedef struct ttracks tTracks;

tTracks* AllocateTracks();
tTracks** ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data, int* tracks_qty);
void ReadTrackArtists(tTracks* track, char* line);
void ReadTrackArtistsIDs(tTracks* track, char* line);
void ReadTrackReleaseDate(tTracks* track, char* line);
void PutFeaturesInArray(tTracks* track);
tTracks** ReallocateMoreTracks(tTracks** tracks, int new_size);
tTracks** ReallocateLessTracks(tTracks** tracks, int old_size, int new_size);
void LinkArtistsToTracks(tSpotfes* spotfes, tTracks** tracks, tArtists** artists);
void ReadBinaryTracks(tTracks** tracks, int quantity);
void SearchTracksByTitle(char* input, tTracks** tracks, int tracks_qty);
void PrintShortTracksDetails(tTracks* track);
void PrintTrackArtists(tTracks* track, int quantity);
void SearchTracksByIndex(int input, tTracks** tracks);
void OpenTrack(tTracks* track);
void ShowPlaylistTracks(tTracks** tracks_from_playlist, int tracks_qty);
tTracks** ReallocateMorePlaylistsTracks(tTracks** tracks, int new_size);
int GetTrackIndex(tTracks* track);
void TrackAddedToPlaylistCounter(tTracks* track);
char* GetTrackName(tTracks* track);
float CalculateAverages(int feature, tTracks** tracks, int tracks_qty);
float GetFeatureValue(tTracks* track, int feature);
float* GetFeatures(tTracks* tracks);
void SaveEuclideanDistanceToTrack(tTracks* track, float euclidean_distance);
float GetDistance(tTracks* tracks);
int GetAddMostAddedTrack(tTracks** tracks, int qty);
int GetTracksAddedCounter(tTracks* track);
void PrintTrackName(FILE* tracks_file, tTracks* track);
void WriteBinaryTracks(tTracks** tracks, int quantity);
void FreeUpTracks(tTracks* tracks);

#endif