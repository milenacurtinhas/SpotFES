#ifndef TSPOTFES_H
#define TSPOTFES_H

typedef struct tspotfes tSpotfes;

void CheckDataFilesPath(int argc);
tSpotfes* AllocateSpotfes();
void FreeUpSpotfes(tSpotfes* spotfes);
void ReadSpotifyDataFiles(tSpotfes* spotfes, char** argv);
int SetUpMainMenu();
int GetArtistsQuantity(tSpotfes* spotfes);
int GetTracksQuantity(tSpotfes* spotfes);

#endif