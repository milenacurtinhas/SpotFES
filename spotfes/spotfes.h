#ifndef TSPOTFES_H
#define TSPOTFES_H

typedef struct tspotfes tSpotfes;

void CheckDataFilesPath(int argc);
tSpotfes* AllocateSpotfes();
void FreeUpSpotfes(tSpotfes* spotfes);
void ReadSpotifyData(tSpotfes* spotfes, char** argv);
int SetUpMainMenu();

#endif