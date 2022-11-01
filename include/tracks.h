#ifndef TTRACKS_H
#define TTRACKS_H

typedef struct ttracks tTracks;

tTracks* AllocateTracks();
void FreeUpTracks(tTracks* tracks);
int ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data);
void LinkArtistsToTracks(tSpotfes* spotfes, tArtists** artists, tTracks** tracks);

#endif