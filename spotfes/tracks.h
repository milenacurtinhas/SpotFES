#ifndef TTRACKS_H
#define TTRACKS_H

typedef struct ttracks tTracks;

tTracks* AllocateTracks();
void FreeUpTracks(tTracks* tracks);

#endif