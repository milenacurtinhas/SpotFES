#include "libraries.h"

int main(int argc, char** argv) {  // ESSE WARNING VAI SAIR QUANDO ATIVARMOS A FUNÇÃO ABAIXO
    // argv[1] = "artists_mm.csv";  // ADICIONAR PARA DEBUGAR
    // argv[2] = "tracks_mm.csv";   // ADICIONAR PARA DEBUGAR
    CheckDataFilesPath(argc, argv);

    tSpotfes* spotfes = NULL;
    spotfes = ReadSpotifyDataFiles(spotfes, argv);

    while (1) {
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