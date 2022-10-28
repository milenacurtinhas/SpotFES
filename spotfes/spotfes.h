#ifndef TSPOTFES_H
#define TSPOTFES_H

typedef struct tspotfes tSpotfes;

void CheckDataFilesPath(int argc);
tSpotfes* AllocateSpotfes();
void ReadSpotifyData(tSpotfes* spotfes, char** argv);
int SetUpMainMenu();
void FreeUpSpotfes(tSpotfes* spotfes);

#endif