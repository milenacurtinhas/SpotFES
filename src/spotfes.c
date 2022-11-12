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
        printf("• ERRO: Não foi possível acessar o arquivo de dados dos artistas. Verifique se ele está no mesmo diretório do executável desse programa.\n\n");
        exit(1);
    }

    FILE* tracks_data = fopen(argv[2], "r");
    if (!tracks_data) {
        printf("• ERRO: Não foi possível acessar o arquivo de dados das músicas. Verifique se ele está no mesmo diretório do executável desse programa.\n\n");
        exit(1);
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

    int input = 0;

    while (scanf("%d%*c", &input) == 0 || !(input >= 1 && input <= 9)) {
        printf("• ERRO: Opção inválida. Tente novamente: ");
    }

    ClearTerminal();

    return input;
}

void QuitProgram(tSpotfes* spotfes) {
    FreeUpSpotfes(spotfes);
    printf("♪ SPOTFES by M&M ♪\n\n");
}

void SearchTracks(tSpotfes* spotfes) {
    char buffer[128], input[128];

    printf("♪ Digite o título de uma música a ser pesquisada: ");
    fgets(buffer, 128, stdin);
    strcpy(input, strtok(buffer, "\n"));
    printf("\n");

    SearchTracksByTitle(input, spotfes->tracks, *spotfes->tracks_qty);
}

void DetailTrack(tSpotfes* spotfes) {
    int input;

    printf("♪ Digite o índice da música a ser detalhada: ");
    while (scanf("%d%*c", &input) == 0 || !(input >= 0 && input <= *spotfes->tracks_qty - 1)) {
        printf("• ERRO: Índice inválido. Tente novamente: ");
    }

    SearchTracksByIndex(input, spotfes->tracks);
}

void CreatePlaylist(tSpotfes* spotfes) {
    if (*spotfes->playlists_qty == *spotfes->playlists_allocs) {  // ESSE TEM QUE SER IGUAL PORQUE O INCREMENTO DE +1 CONTA PARA A PRÓXIMA PLAYLIST
        *spotfes->playlists_allocs *= 2;                          // DAÍ QUANDO A QUANTIDADE FOR 16, SERÃO 17 PLAYLISTS, MAIOR QUE O NÚMERO DE MALLOCS
        spotfes->playlists = ReallocateMorePlaylists(spotfes->playlists, *spotfes->playlists_allocs);
    }

    char buffer[128], input[128];

    printf("♪ Digite o nome da playlist a ser criada: ");
    fgets(buffer, 128, stdin) == NULL;
    strcpy(input, strtok(buffer, "\n"));
    printf("\n");

    NewPlaylist(input, spotfes->playlists, *spotfes->playlists_qty);
    *spotfes->playlists_qty += 1;
}

void ListPlaylists(tSpotfes* spotfes) {
    DisplayPlaylists(spotfes->playlists, *spotfes->playlists_qty);
}

void DetailPlaylist(tSpotfes* spotfes) {
    int input;
    int playlists_qty = GetPlaylistsQuantity(spotfes);

    if (!playlists_qty) {
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
    } else {
        printf("♪ Digite o índice da playlist a ser listada: ");
        while (scanf("%d%*c", &input) == 0 || !(input >= 0 && input <= *spotfes->playlists_qty - 1)) {
            printf("• ERRO: Índice inválido. Tente novamente: ");
        }

        SearchPlaylistByIndex(input, spotfes->playlists);
    }
}

void AddTrackToPlaylist(tSpotfes* spotfes) {
    int track_index, playlist_index;
    int playlists_qty = GetPlaylistsQuantity(spotfes);

    if (!playlists_qty) {
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
    } else {
        printf("♪ Digite o índice da música a ser adicionada e o índice da playlist alvo: ");
        while (scanf("%d %d%*c", &track_index, &playlist_index) == 0 || !(track_index >= 0 && track_index <= *spotfes->tracks_qty) || !(playlist_index >= 0 && playlist_index <= *spotfes->playlists_qty - 1)) {
            if (!(track_index >= 0 && track_index <= *spotfes->tracks_qty)) {
                printf("• ERRO: Índice de música inválido. Tente novamente: ");
            } else if (!(playlist_index >= 0 && playlist_index <= *spotfes->playlists_qty - 1)) {
                printf("• ERRO: Índice de playlist inválido. Tente novamente: ");
            } else {
                printf("• ERRO: Tente novamente: ");
            }
        }

        LinkTrackToPlaylist(spotfes->playlists[playlist_index], spotfes->tracks[track_index]);
    }
}

void RecommendSimilarTrack(tSpotfes* spotfes) {
    int tracks_qty = GetTracksQuantity(spotfes);
    int playlists_qty = GetPlaylistsQuantity(spotfes);
    int playlist_index, qty;

    if (!playlists_qty) {
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
    } else {
        printf("♪ Digite o índice da playlist alvo e a quantidade de músicas similares a serem recomendadas: ");
        while (scanf("%d %d%*c", &playlist_index, &qty) != 2 || !(playlist_index >= 0 && playlist_index <= playlists_qty) || !(qty >= 1 && qty <= tracks_qty)) {
            int playlist_tracks_qty = GetPlaylistAddedTracksQuantity(spotfes, playlist_index);

            if (!playlist_tracks_qty) {
                printf("\n• ERRO: A playlist está vazia.\n\n");
            } else {
                if (!(playlist_index >= 0 && playlist_index <= playlists_qty)) {
                    printf("• ERRO: Índice de playlist inválido. Tente novamente: ");
                } else if (!(qty >= 1)) {
                    printf("• ERRO: O número de recomendações deve ser de no mínimo 1. Tente novamente: ");
                } else if (!(qty <= tracks_qty)) {
                    printf("• ERRO: O número de recomendações foi maior do que o número de músicas do programa. Tente novamente: ");
                }
            }
        }

        int playlist_tracks_qty = GetPlaylistAddedTracksQuantity(spotfes, playlist_index);

        if (!playlist_tracks_qty) {
            printf("\n• ERRO: A playlist está vazia.\n\n");
        } else {
            ComparePlaylistToTracks(spotfes, spotfes->playlists[playlist_index], qty);
        }
    }
}

int GetArtistsQuantity(tSpotfes* spotfes) {
    return *spotfes->artists_qty;
}

int GetTracksQuantity(tSpotfes* spotfes) {
    return *spotfes->tracks_qty;
}

int GetPlaylistsQuantity(tSpotfes* spotfes) {
    return *spotfes->playlists_qty;
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
    int artists_qty = GetTrackArtistsQuantity(spotfes->tracks[index]);

    PrintShortTracksDetails(spotfes->tracks[index], artists_qty);
    printf("\n");
}