#include "libraries.h"

struct tspotfes {
    tArtists** artists;
    int* artists_qty;
    tTracks** tracks;
    int* tracks_qty;
    tPlaylists** playlists;
    int* playlists_qty;
    int* playlists_allocs;
};

tSpotfes* AllocateSpotfes(tSpotfes* spotfes) {
    spotfes = (tSpotfes*)malloc(sizeof(tSpotfes));

    spotfes->artists = (tArtists**)malloc(sizeof(tArtists*) * 128);
    spotfes->artists_qty = (int*)malloc(sizeof(int));
    *spotfes->artists_qty = 0;

    spotfes->tracks = (tTracks**)malloc(sizeof(tTracks*) * 128);
    spotfes->tracks_qty = (int*)malloc(sizeof(int));
    *spotfes->tracks_qty = 0;

    spotfes->playlists = (tPlaylists**)malloc(sizeof(tPlaylists*) * 16);
    spotfes->playlists_qty = (int*)malloc(sizeof(int));
    spotfes->playlists_allocs = (int*)malloc(sizeof(int));
    *spotfes->playlists_qty = 0;
    *spotfes->playlists_allocs = 16;

    for (int m = 0; m < 128; m++) {
        spotfes->artists[m] = AllocateArtists();
        spotfes->tracks[m] = AllocateTracks();

        if (m < 16) {
            spotfes->playlists[m] = AllocatePlaylists();
        }
    }

    return spotfes;
}

void FreeUpSpotfes(tSpotfes* spotfes) {
    for (int m = 0; m < *spotfes->artists_qty; m++) {
        FreeUpArtists(spotfes->artists[m]);
    }

    for (int m = 0; m < *spotfes->tracks_qty; m++) {
        FreeUpTracks(spotfes->tracks[m]);
    }

    for (int m = 0; m < *spotfes->playlists_allocs; m++) {
        FreeUpPlaylists(spotfes->playlists[m]);
    }

    FreeAndNullPointer(spotfes->artists);
    FreeAndNullPointer(spotfes->artists_qty);
    FreeAndNullPointer(spotfes->tracks);
    FreeAndNullPointer(spotfes->tracks_qty);
    FreeAndNullPointer(spotfes->playlists);
    FreeAndNullPointer(spotfes->playlists_qty);
    FreeAndNullPointer(spotfes->playlists_allocs);
    FreeAndNullPointer(spotfes);
}

tSpotfes* ReadSpotifyDataFiles(tSpotfes* spotfes, char** argv) {
    FILE* artists_data = fopen(argv[1], "r");
    if (!artists_data) {
        PrintMissingFilesErrorAndQuitProgram();
    }

    FILE* tracks_data = fopen(argv[2], "r");
    if (!tracks_data) {
        PrintMissingFilesErrorAndQuitProgram();
    }

    spotfes = AllocateSpotfes(spotfes);

    spotfes->artists = ReadArtistsDataFiles(spotfes->artists, artists_data, spotfes->artists_qty);
    fclose(artists_data);

    spotfes->tracks = ReadTracksDataFiles(spotfes->tracks, tracks_data, spotfes->tracks_qty);
    fclose(tracks_data);

    LinkArtistsToTracks(spotfes, spotfes->tracks, spotfes->artists);

    return spotfes;
}

int SetUpMainMenu() {
    printf("|---------------------------------------------------|\n");
    printf("| ♪ | SPOTFES by M&M                                |\n");
    printf("|---------------------------------------------------|\n");
    printf("| 1 | Pesquisar músicas                             |\n");
    printf("| 2 | Detalhar uma música                           |\n");
    printf("| 3 | Criar uma playlist                            |\n");
    printf("| 4 | Exibir playlists                              |\n");
    printf("| 5 | Detalhar uma playlist                         |\n");
    printf("| 6 | Adicionar uma música na playlist              |\n");
    printf("| 7 | Recomendar músicas parecidas com uma playlist |\n");
    printf("| 8 | Gerar relatório                               |\n");
    printf("| 9 | Sair do programa                              |\n");
    printf("|---------------------------------------------------|\n\n");
    printf("♪ Digite a opção desejada: ");

    int input = GetValidIntegerInput(1, 9);
    ClearTerminal();

    return input;
}

void QuitProgram(tSpotfes* spotfes) {
    FreeUpSpotfes(spotfes);
    printf("SpotFES by M&M: no warnings, no leaks, no errors ♪\n");
}

void SearchTracks(tSpotfes* spotfes) {
    char buffer[128], input[128];

    printf("♪ Digite o título de uma música a ser pesquisada: ");
    if (fgets(buffer, 128, stdin) != NULL) {
        strcpy(input, strtok(buffer, "\n"));
        printf("\n");

        SearchTracksByTitle(input, spotfes->tracks, *spotfes->tracks_qty);
    }
}

void DetailTrack(tSpotfes* spotfes) {
    printf("♪ Digite o índice da música a ser detalhada: ");
    int input = GetValidIntegerInput(0, *spotfes->tracks_qty - 1);

    SearchTracksByIndex(input, spotfes->tracks);
}

void CreatePlaylist(tSpotfes* spotfes) {
    if (*spotfes->playlists_qty == *spotfes->playlists_allocs) {  // ESSE TEM QUE SER IGUAL PORQUE O INCREMENTO DE +1 CONTA PARA A PRÓXIMA PLAYLIST
        *spotfes->playlists_allocs *= 2;                          // DAÍ QUANDO A QUANTIDADE FOR 16, SERÃO 17 PLAYLISTS, MAIOR QUE O NÚMERO DE MALLOCS
        spotfes->playlists = ReallocateMorePlaylists(spotfes->playlists, *spotfes->playlists_allocs);
    }

    char buffer[128], input[128];

    printf("♪ Digite o nome da playlist a ser criada: ");
    if (fgets(buffer, 128, stdin)) {
        strcpy(input, strtok(buffer, "\n"));
        printf("\n");

        NewPlaylist(input, spotfes->playlists, *spotfes->playlists_qty);
        *spotfes->playlists_qty += 1;
        printf("A playlist '%s' foi criada com sucesso.\n\n", input);

    }
}

void ListPlaylists(tSpotfes* spotfes) {
    DisplayPlaylists(spotfes->playlists, *spotfes->playlists_qty);
}

void DetailPlaylist(tSpotfes* spotfes) {
    if (!*spotfes->playlists_qty) {
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
    } else {
        printf("♪ Digite o índice da playlist a ser listada: ");
        int input = GetValidIntegerInput(0, *spotfes->playlists_qty - 1);

        SearchPlaylistByIndex(input, spotfes->playlists);
    }
}

void AddTrackToPlaylist(tSpotfes* spotfes) {
    if (!*spotfes->playlists_qty) {
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
    } else {
        printf("♪ Digite o índice da música a ser adicionada: ");
        int track_index = GetValidIntegerInput(0, *spotfes->tracks_qty - 1);

        printf("♪ Digite o índice da playlist alvo: ");
        int playlist_index = GetValidIntegerInput(0, *spotfes->playlists_qty - 1);

        LinkTrackToPlaylist(spotfes->playlists[playlist_index], spotfes->tracks[track_index]);

        char* track_name = strdup(GetTrackName(spotfes->tracks[track_index]));
        char* playlist_name = strdup(GetPlaylistName(spotfes->playlists[playlist_index]));

        printf("\nA música '%s' foi adicionada à playlist '%s' com sucesso.\n\n", track_name, playlist_name);

        FreeAndNullPointer(track_name);
        FreeAndNullPointer(playlist_name);
    }
}

void RecommendSimilarTrack(tSpotfes* spotfes) {
    if (!*spotfes->playlists_qty) {
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
    } else {
        printf("♪ Digite o índice da playlist alvo: ");
        int playlist_index = GetValidIntegerInput(0, *spotfes->playlists_qty - 1);

        int playlist_tracks_qty = GetPlaylistAddedTracksQuantity(spotfes, playlist_index);
        if (!playlist_tracks_qty) {
            printf("\n• ERRO: A playlist está vazia.\n\n");
        } else {
            printf("♪ Digite o número de músicas semelhantes a serem recomendadas: ");
            int quantity = GetValidIntegerInput(1, *spotfes->tracks_qty);
            ComparePlaylistToTracks(spotfes, spotfes->playlists[playlist_index], quantity);
        }
    }
}

int GetArtistsQuantity(tSpotfes* spotfes) {
    return *spotfes->artists_qty;
}

int GetTracksQuantity(tSpotfes* spotfes) {
    return *spotfes->tracks_qty;
}

int GetPlaylistAddedTracksQuantity(tSpotfes* spotfes, int index) {
    int qty = GetPlaylistTracksQuantity(spotfes->playlists[index]);

    return qty;
}

float* GetTracksFeatures(tSpotfes* spotfes, int m) {
    return GetFeatures(spotfes->tracks[m]);
}

void RelatesDistanceToTrack(tSpotfes* spotfes, float* euclidean_distance) {
    for (int m = 0; m < *spotfes->tracks_qty; m++) {
        SaveEuclideanDistanceToTrack(spotfes->tracks[m], euclidean_distance[m]);
    }
}

float GetTrackDistance(tSpotfes* spotfes, int index) {
    float distance = GetDistance(spotfes->tracks[index]);

    return distance;
}

void PrintSimilarTrack(tSpotfes* spotfes, int index) {
    PrintShortTracksDetails(spotfes->tracks[index]);
    printf("\n");
}

void GenerateReport(tSpotfes* spotfes) {
    /*FILE * tracks_file;
    FILE * artists_file;

    tracks_file = fopen ("relatório/músicas.txt", "rw");
    artists_file = fopen ("relatório/artistas.txt", "rw");*/

    int most_added_tracks = GetAddMostAddedTrack (spotfes->tracks, *spotfes->tracks_qty);
    int most_added_atists = GetAddMostAddedArtist (spotfes->artists, *spotfes->artists_qty);

    for (int m = most_added_tracks; m > 0; m--) {
        for (int mm = 0; mm < *spotfes->tracks_qty; mm++) {
            if (GetTracksAddedCounter (spotfes->tracks[mm]) == m) {
                PrintTrackName (spotfes->tracks[mm]);
            }
        }
    }

    for (int m = most_added_atists; m > 0; m--) {
        for (int mm = 0; mm < *spotfes->artists_qty; mm++) {
            if (GetArtistAddedCounter (spotfes->artists[mm]) == m) {
                PrintArtistName (spotfes->artists[mm]);
            }
        }
    }

    /*fclose (tracks_file);
    fclose (artists_file);*/
}