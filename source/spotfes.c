#include "libraries.h"

/**
 * @brief struct usada para agrupar todas as informações do programa
 */
struct tspotfes {
    tArtists** artists;
    int* artists_qty;
    tTracks** tracks;
    int* tracks_qty;
    tPlaylists** playlists;
    int* playlists_qty;
    int* playlists_allocs;
};

/**
 * @brief inicializa o SpotFES a partir da leitura dos arquivos .csv e dos arquivos binários
 * @return uma struct do tipo tSpotfes com todas as informações lidas
 */
tSpotfes* ReadSpotfesDataFiles(tSpotfes* spotfes, char** argv) {
    FILE* artists_data = fopen(argv[1], "r");
    if (!artists_data) {
        PrintDataFilesError();
        exit(1);
    }

    FILE* tracks_data = fopen(argv[2], "r");
    if (!tracks_data) {
        PrintDataFilesError();
        exit(1);
    }

    spotfes = AllocateSpotfes(spotfes);

    spotfes->artists = ReadArtistsDataFiles(spotfes->artists, artists_data, spotfes->artists_qty);
    FreeAndNullPointer(argv[1]);
    fclose(artists_data);

    spotfes->tracks = ReadTracksDataFiles(spotfes->tracks, tracks_data, spotfes->tracks_qty);
    FreeAndNullPointer(argv[2]);
    fclose(tracks_data);

    LinkArtistsToTracks(spotfes, spotfes->tracks, spotfes->artists);
    LinkBinaryTracksToPlaylists(spotfes->playlists, *spotfes->playlists_qty, spotfes->tracks);
    ReadBinaryArtists(spotfes->artists, *spotfes->artists_qty);
    ReadBinaryTracks(spotfes->tracks, *spotfes->tracks_qty);

    return spotfes;
}

/**
 * @brief alloca os blocos de memória iniciais do Spotfes
 * @return ponteiro do tipo Spotfes conténdo as alocações de memória das playlists, músicas e artistas
 */
tSpotfes* AllocateSpotfes(tSpotfes* spotfes) {
    spotfes = (tSpotfes*)malloc(sizeof(tSpotfes));

    spotfes->artists = (tArtists**)malloc(sizeof(tArtists*) * STARTER_DATA_SIZE);
    spotfes->artists_qty = (int*)malloc(sizeof(int));
    *spotfes->artists_qty = 0;

    spotfes->tracks = (tTracks**)malloc(sizeof(tTracks*) * STARTER_DATA_SIZE);
    spotfes->tracks_qty = (int*)malloc(sizeof(int));
    *spotfes->tracks_qty = 0;

    for (int m = 0; m < STARTER_DATA_SIZE; m++) {
        spotfes->artists[m] = AllocateArtists();
        spotfes->tracks[m] = AllocateTracks();
    }

    spotfes->playlists_qty = (int*)malloc(sizeof(int));
    spotfes->playlists_allocs = (int*)malloc(sizeof(int));
    *spotfes->playlists_qty = 0;
    *spotfes->playlists_allocs = STARTER_PLAYLISTS_SIZE;
    spotfes->playlists = AllocatePlaylists(spotfes->playlists_qty, spotfes->playlists_allocs);

    return spotfes;
}

int GetArtistsQuantity(tSpotfes* spotfes) {
    return *spotfes->artists_qty;
}

int GetTracksQuantity(tSpotfes* spotfes) {
    return *spotfes->tracks_qty;
}

/**
 * @brief exibe o menu
 * @return a opção escolhida no menu
 */
int SetUpMainMenu() {
    PrintMenuArt();
    BLACK_COLOUR;
    printf("1 • PESQUISAR MÚSICAS\n");
    printf("2 • DETALHAR UMA MÚSICA\n");
    printf("3 • CRIAR UMA PLAYLIST\n");
    printf("4 • EXIBIR PLAYLISTS\n");
    printf("5 • DETALHAR UMA PLAYLIST\n");
    printf("6 • ADICIONAR UMA MÚSICA NA PLAYLIST\n");
    printf("7 • RECOMENDAR MÚSICAS PARECIDAS COM UMA PLAYLIST \n");
    printf("8 • GERAR RELATÓRIO SOBRE AS PLAYLISTS\n");
    printf("9 • SAIR DO PROGRAMA\n\n");
    NORMAL_COLOUR;
    printf("♪ Digite a opção desejada: ");

    int input = GetValidIntegerInput(1, 9);
    ClearTerminal();
    BLACK_COLOUR;

    return input;
}

/**
 * @brief pesquisa músicas a partir de uma palavra-chave digitada pelo usuário
 */
void SearchTracks(tSpotfes* spotfes) {
    char buffer[STRING_BUFFER_SIZE], input[STRING_BUFFER_SIZE];

    printf("♪ Digite o título de uma música a ser pesquisada: ");

    if (fgets(buffer, STRING_BUFFER_SIZE, stdin) != NULL) {
        strcpy(input, strtok(buffer, "\n"));
        printf("\n");

        NORMAL_COLOUR;
        SearchTracksByTitle(input, spotfes->tracks, *spotfes->tracks_qty);
    }
}

/**
 * @brief exibe todas as características armazenadas de uma música a partir do índice dela
 */
void DetailTrack(tSpotfes* spotfes) {
    printf("♪ Digite o índice da música a ser detalhada: ");

    int input = GetValidIntegerInput(0, *spotfes->tracks_qty - 1);

    NORMAL_COLOUR;
    SearchTracksByIndex(input, spotfes->tracks);
}

void CreatePlaylist(tSpotfes* spotfes) {
    if (*spotfes->playlists_qty == *spotfes->playlists_allocs) {
        *spotfes->playlists_allocs *= 2;
        spotfes->playlists = ReallocateMorePlaylists(spotfes->playlists, *spotfes->playlists_allocs);
    }

    char buffer[STRING_BUFFER_SIZE], input[STRING_BUFFER_SIZE];

    printf("♪ Digite o nome da playlist a ser criada: ");

    if (fgets(buffer, STRING_BUFFER_SIZE, stdin)) {
        strcpy(input, strtok(buffer, "\n"));
        printf("\n");

        NewPlaylist(input, spotfes->playlists, *spotfes->playlists_qty);
        *spotfes->playlists_qty += 1;

        NORMAL_COLOUR;
        printf("A playlist '%s' foi criada com sucesso.\n\n", input);
    }
}

/**
 * @brief exibe todas as playlists criadas
 */
void ListPlaylists(tSpotfes* spotfes) {
    DisplayPlaylists(spotfes->playlists, *spotfes->playlists_qty);
}

/**
 * @brief exibe todas as características armazenadas de uma playlist a partir do índice dela
 */
void DetailPlaylist(tSpotfes* spotfes) {
    if (!*spotfes->playlists_qty) {
        RED_COLOUR;
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
        NORMAL_COLOUR;
    } else {
        printf("♪ Digite o índice da playlist a ser listada: ");
        int input = GetValidIntegerInput(0, *spotfes->playlists_qty - 1);
        NORMAL_COLOUR;

        SearchPlaylistByIndex(input, spotfes->playlists);
    }
}

/**
 * @brief adiciona uma música específica em uma playlist específica a partir do índice de cada uma
 */
void AddTrackToPlaylist(tSpotfes* spotfes) {
    if (!*spotfes->playlists_qty) {
        RED_COLOUR;
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
    } else {
        printf("♪ Digite o índice da música a ser adicionada: ");
        int track_index = GetValidIntegerInput(0, *spotfes->tracks_qty - 1);

        printf("♪ Digite o índice da playlist alvo: ");
        int playlist_index = GetValidIntegerInput(0, *spotfes->playlists_qty - 1);

        spotfes->playlists[playlist_index] = LinkTrackToPlaylist(spotfes->playlists[playlist_index], spotfes->tracks[track_index]);

        char* track_name = strdup(GetTrackName(spotfes->tracks[track_index]));
        char* playlist_name = strdup(GetPlaylistName(spotfes->playlists[playlist_index]));

        NORMAL_COLOUR;
        printf("\nA música '%s' foi adicionada à playlist '%s' com sucesso.\n\n", track_name, playlist_name);

        FreeAndNullPointer(track_name);
        FreeAndNullPointer(playlist_name);
    }
}

/**
 * @brief recomenda músicas semelhantes às músicas de uma playlist a partir do algoritmo de recomendação
 */
void RecommendSimilarTrack(tSpotfes* spotfes) {
    if (!*spotfes->playlists_qty) {
        RED_COLOUR;
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
    } else {
        printf("♪ Digite o índice da playlist alvo: ");

        int playlist_index = GetValidIntegerInput(0, *spotfes->playlists_qty - 1);
        int playlist_tracks_qty = GetPlaylistTracksQuantity(spotfes->playlists[playlist_index]);

        if (!playlist_tracks_qty) {
            RED_COLOUR;
            printf("\n• ERRO: A playlist está vazia.\n\n");
        } else {
            printf("♪ Digite o número de músicas semelhantes a serem recomendadas: ");
            int quantity = GetValidIntegerInput(1, *spotfes->tracks_qty);
            NORMAL_COLOUR;
            ComparePlaylistToTracks(spotfes, spotfes->playlists[playlist_index], quantity);
        }
    }
}

/**
 * @brief retorna o vetor que contém as características da música usadas para calcular a distância euclidiana
 * @param m índice da música
 * @return vetor que contém as características da música usadas para calcular a distância euclidiana
 */
float* GetTracksFeatures(tSpotfes* spotfes, int m) {
    return GetFeatures(spotfes->tracks[m]);
}

/**
 * @brief salva a distância euclidiana calculada entre a música e uma playlist na struct da música
 */
void RelatesDistanceToTrack(tSpotfes* spotfes, float* euclidean_distance) {
    for (int m = 0; m < *spotfes->tracks_qty; m++) {
        SaveEuclideanDistanceToTrack(spotfes->tracks[m], euclidean_distance[m]);
    }
}

/**
 * @brief retorna a distância euclidiana calculada da música com uma playlist
 * @return valor da distância euclidiana
 */
float GetTrackDistance(tSpotfes* spotfes, int index) {
    return GetDistance(spotfes->tracks[index]);
}

/**
 * @brief imprime as recomendações de músicas semelhantes
 */
void PrintSimilarTrack(tSpotfes* spotfes, int index) {
    PrintShortTracksDetails(spotfes->tracks[index]);
    printf("\n");
}

/**
 * @brief gera um relatório em forma de dois arquivos .txt, um contendo as músicas salvas nas playlists e outro contendo os artistas cujas músicas foram salvas nas playlists
 */
void GenerateReport(tSpotfes* spotfes) {
    if (!*spotfes->playlists_qty) {
        RED_COLOUR;
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
        NORMAL_COLOUR;
    } else {
        FILE* tracks_file = fopen("reports/tracks.txt", "w");
        FILE* artists_file = fopen("reports/artists.txt", "w");

        int most_added_tracks = GetAddMostAddedTrack(spotfes->tracks, *spotfes->tracks_qty);
        int most_added_artists = GetAddMostAddedArtist(spotfes->artists, *spotfes->artists_qty);

        if (!most_added_tracks && !most_added_artists) {
            RED_COLOUR;
            printf("• ERRO: Todas as playlists estão vazias.\n\n");
        } else {
            // começa a iteração a partir do número correspondente à música com maior número de aparições em playlists, imprimindo as músicas em ordem decrescente de aparições
            for (int m = most_added_tracks; m > 0; m--) {
                for (int mm = 0; mm < *spotfes->tracks_qty; mm++) {
                    if (GetTracksAddedCounter(spotfes->tracks[mm]) == m) {
                        PrintTrackName(tracks_file, spotfes->tracks[mm]);
                    }
                }
            }
            // começa a iteração a partir do número correspondente ao artista com maior número de aparições em playlists, imprimindo os artistas em ordem decrescente de aparições
            for (int m = most_added_artists; m > 0; m--) {
                for (int mm = 0; mm < *spotfes->artists_qty; mm++) {
                    if (GetArtistAddedCounter(spotfes->artists[mm]) == m) {
                        PrintArtistName(artists_file, spotfes->artists[mm]);
                    }
                }
            }
            NORMAL_COLOUR;
            printf("Relatórios gerados com sucesso na pasta 'reports'.\n\n");
        }
        fclose(tracks_file);
        fclose(artists_file);
    }
}

/**
 * @brief salva os dados do programa em arquivos binários, libera a memória utilizada e encerra o programa
 */
void QuitProgram(tSpotfes* spotfes) {
    WriteBinaryPlaylists(spotfes->playlists, *spotfes->playlists_qty);
    WriteBinaryArtists(spotfes->artists, *spotfes->artists_qty);
    WriteBinaryTracks(spotfes->tracks, *spotfes->tracks_qty);
    FreeUpSpotfes(spotfes);
    PrintMenuArt();
    printf("no warnings, no leaks, no errors :)\n\n");
}

/**
 * @brief libera os blocos de memória alocados para cada struct
 */
void FreeUpSpotfes(tSpotfes* spotfes) {
    for (int m = 0; m < *spotfes->playlists_allocs; m++) {
        FreeUpPlaylists(spotfes->playlists[m]);
    }

    for (int m = 0; m < *spotfes->tracks_qty; m++) {
        FreeUpTracks(spotfes->tracks[m]);
    }

    for (int m = 0; m < *spotfes->artists_qty; m++) {
        FreeUpArtists(spotfes->artists[m]);
    }

    FreeAndNullPointer(spotfes->playlists_allocs);
    FreeAndNullPointer(spotfes->playlists_qty);
    FreeAndNullPointer(spotfes->playlists);
    FreeAndNullPointer(spotfes->tracks_qty);
    FreeAndNullPointer(spotfes->tracks);
    FreeAndNullPointer(spotfes->artists_qty);
    FreeAndNullPointer(spotfes->artists);
    FreeAndNullPointer(spotfes);
}
