#include "libraries.h"

int main(int argc, char** argv) {
    // CheckDataFilesPath(argc); // adicionar ao terminar
    argv[1] = "data/artists_mm.csv";  // remover ao terminar
    argv[2] = "data/tracks_mm.csv";   // remover ao terminar

    tSpotfes* spotfes = AllocateSpotfes();

    ReadSpotifyDataFiles(spotfes, argv);  // acho que poderiamos modularizar essa função em duas separando o scan dos artistas e o scan das músicas

    int input = SetUpMainMenu(input);
    switch (input) {
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
            ListPlaylist(spotfes);
            break;
        case 6:
            AddTrackToPlaylist(spotfes);
            break;
        case 7:
            //RecommendSimilarTrack(spotfes);
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