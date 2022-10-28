#include "libraries.h"

int main(int argc, char** argv) {
    //CheckDataFilesPath(argc);
    argv[1] = "artists_2.csv";
    argv[2] = "tracks_2.csv";
    tSpotfes* spotfes = AllocateSpotfes();
    ReadSpotifyData(spotfes, argv);

    int input = SetUpMainMenu();

    switch (input) {
        case 1:
            // SearchTracks(spotfes);
            break;
        case 2:
            // ListTrack(spotfes);
            // PlayTrack(spotfes);
            break;
        case 3:
            // CreatePlaylist(spotfes);
            break;
        case 4:
            // ListPlaylists(spotfes);
            break;
        case 5:
            // ListPlaylist(spotfes);
            break;
        case 6:
            // AddTrackToPlaylist(spotfes);
            break;
        case 7:
            // RecommendSimilarTrack(spotfes);
            break;
        case 8:
            // GenerateReport(spotfes);
            break;
        default:
            exit(1);
    }

    FreeUpSpotfes(spotfes);

    return 0;
}