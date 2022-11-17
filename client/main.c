#include "libraries.h"

int main(int argc, char** argv) {
    CheckDataFilesPath(argc, argv);

    tSpotfes* spotfes = NULL;
    spotfes = ReadSpotfesDataFiles(spotfes, argv);

    while (ONLINE) {
        switch (SetUpMainMenu()) {
            case 1:
                SearchTracks(spotfes);
                break;
            case 2:
                DetailTrack(spotfes);
                break;
            case 3:
                CreatePlaylist(spotfes);
                break;
            case 4:
                ListPlaylists(spotfes);
                break;
            case 5:
                DetailPlaylist(spotfes);
                break;
            case 6:
                AddTrackToPlaylist(spotfes);
                break;
            case 7:
                RecommendSimilarTrack(spotfes);
                break;
            case 8:
                GenerateReport(spotfes);
                break;
            case 9:
                QuitProgram(spotfes);
                return 0;
        }
    }
}