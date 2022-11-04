#ifndef TTRACKS_H
#define TTRACKS_H

typedef struct ttracks tTracks;

tTracks* AllocateTracks();
tTracks** ReallocateMoreTracks(tTracks** tracks, int new_size);
tTracks** ReallocateLessTracks(tTracks** tracks, int old_size, int* new_size);
void FreeUpTracks(tTracks* tracks);
tTracks** ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data, int* tracks_qty);
void LinkArtistsToTracks(tSpotfes* spotfes, tTracks** tracks, tArtists** artists);

#endif