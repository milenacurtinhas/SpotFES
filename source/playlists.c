#include "libraries.h"

/**
 * @brief struct usada para guardar informações sobre playlists
 *
 */
struct tplaylists {
    int index;
    int name_size;
    char* playlist_name;
    int tracks_qty;
    int* tracks_index;
    tTracks** tracks;
    float* averages;  // média das oito características das músicas contidas na playlist usadas para calcular a distância euclidina
    int* tracks_alloc_size;
};

/**
 * @brief salva as informações iniciais da(s) playlist(s)
 *
 * @param playlists_qty um ponteiro que armazenará a quantidade de playlists lidas caso haja arquivos binários
 * @param playlists_allocs o tamanho inicial padrão para a alocação de playlists
 * @return um vetor de structs do tipo tPlaylists com as informações lidas
 */
tPlaylists** AllocatePlaylists(int* playlists_qty, int* playlists_allocs) {
    FILE* playlists_file = fopen("binaries/playlists.bin", "rb");

    // caso não haja arquivos binários com as informações das playlists anteriormente criadas, a inicialização será feita com os valores padrões
    if (playlists_file == NULL) {
        tPlaylists** playlists = (tPlaylists**)calloc(sizeof(tPlaylists*), STARTER_PLAYLISTS_SIZE);

        for (int m = 0; m < STARTER_PLAYLISTS_SIZE; m++) {
            playlists[m] = AllocatePlaylist();
        }

        return playlists;
    } else {
        return ReadBinaryPlaylists(playlists_file, playlists_qty, playlists_allocs);
    }
}

/**
 * @brief alloca os blocos de memória iniciais da playlist
 *
 * @return ponteiro do tipo Playlists conténdo as alocações de memória das playlists, músicas e artistas
 */
tPlaylists* AllocatePlaylist() {
    tPlaylists* playlist = (tPlaylists*)calloc(sizeof(tPlaylists), 1);
    playlist->playlist_name = (char*)calloc(sizeof(char), STRING_BUFFER_SIZE);
    playlist->tracks_index = (int*)calloc(sizeof(int), STARTER_PLAYLISTS_SIZE);
    playlist->tracks = (tTracks**)calloc(sizeof(tTracks*), STARTER_PLAYLISTS_SIZE);
    playlist->averages = (float*)calloc(sizeof(float), TRACK_STATS_QUANTITY);
    playlist->tracks_alloc_size = (int*)calloc(sizeof(int), 1);
    *playlist->tracks_alloc_size = STARTER_PLAYLISTS_SIZE;

    return playlist;
}

/**
 * @brief lê o arquivo binário das playlists previamente inicializadas
 *
 * @param playlists_file o arquivo binário com as informações
 * @param playlists_qty um ponteiro que armazenará a quantidade de playlists lidas caso haja arquivos binários
 * @param playlists_allocs o tamanho inicial padrão para a alocação de playlists
 * @return um vetor de structs do tipo tPlaylists com as informações lidas
 */
tPlaylists** ReadBinaryPlaylists(FILE* playlists_file, int* playlists_qty, int* playlists_allocs) {
    size_t read = fread(playlists_qty, sizeof(int), 1, playlists_file);

    // dobra o tamanho do alloc das playlists enquanto for menor que o tamanho do alloc inicial padrão
    while (*playlists_qty > *playlists_allocs) {
        *playlists_allocs *= 2;
    }

    tPlaylists** playlists = (tPlaylists**)calloc(sizeof(tPlaylists*), *playlists_allocs);

    for (int m = 0; m < *playlists_allocs; m++) {
        playlists[m] = AllocatePlaylist();
    }

    for (int m = 0; m < *playlists_qty; m++) {
        read += fread(&playlists[m]->index, sizeof(int), 1, playlists_file);
        read += fread(&playlists[m]->name_size, sizeof(int), 1, playlists_file);
        read += fread(playlists[m]->playlist_name, sizeof(char), playlists[m]->name_size, playlists_file);
        read += fread(&playlists[m]->tracks_qty, sizeof(int), 1, playlists_file);

        // faz o realloc caso a quantidade de tracks for maior que o alloc inicial padrão
        if (playlists[m]->tracks_qty > *playlists[m]->tracks_alloc_size) {
            while (playlists[m]->tracks_qty > *playlists[m]->tracks_alloc_size) {
                *playlists[m]->tracks_alloc_size *= 2;
            }

            tTracks** new_tracks = NULL;
            int* new_index = NULL;

            new_index = (int*)realloc(playlists[m]->tracks_index, sizeof(int) * *playlists[m]->tracks_alloc_size);
            new_tracks = (tTracks**)realloc(playlists[m]->tracks, sizeof(tTracks*) * *playlists[m]->tracks_alloc_size);

            playlists[m]->tracks_index = new_index;
            playlists[m]->tracks = new_tracks;
        }

        read += fread(playlists[m]->tracks_index, sizeof(int), playlists[m]->tracks_qty, playlists_file);
        read += fread(playlists[m]->averages, sizeof(float), TRACK_STATS_QUANTITY, playlists_file);
        read += fread(playlists[m]->tracks_alloc_size, sizeof(int), 1, playlists_file);
    }

    // se a função fread() não ler nada, ela não retornará nada para a variável 'read' que terá o valor zerado e encerrará o programa pelo if/else
    if (read) {
        fclose(playlists_file);
        return playlists;
    } else {
        PrintBinaryFilesError();
        exit(1);
    }
}

/**
 * @brief guarda na playlist o ponteiro das músicas correspondentes aos índices das músicas salvas no array de índices
 * @param playlists todas as playlists lidas do arquivo binário
 * @param playlists_qty quantidade de playlists lidas do arquivo binário
 * @param tracks todas as músicas lidas do arquivo .csv
 */
void LinkBinaryTracksToPlaylists(tPlaylists** playlists, int playlists_qty, tTracks** tracks) {
    for (int m = 0; m < playlists_qty; m++) {
        for (int mm = 0; mm < playlists[m]->tracks_qty; mm++) {
            int target_index = playlists[m]->tracks_index[mm];
            playlists[m]->tracks[mm] = tracks[target_index];
        }
    }
}

/**
 * @brief cria nova playlist
 * @param input nome da nova playlist
 * @param playlists_qty index da playlist
 */
void NewPlaylist(char* input, tPlaylists** playlists, int playlists_qty) {
    playlists[playlists_qty]->index = playlists_qty;
    strcpy(playlists[playlists_qty]->playlist_name, input);
    playlists[playlists_qty]->name_size = strlen(input);
}

/**
 * @brief realloca o ponteiro de playlists dobrando a capacidade de armazenamento
 * @param new_size o tamanho do alloc anterior dobrado
 * @return um ponteiro contendo as playlists já lidos e os espaços para as novas playlists a serem lidos
 */
tPlaylists** ReallocateMorePlaylists(tPlaylists** playlists, int new_size) {
    tPlaylists** new = NULL;
    new = (tPlaylists**)realloc(playlists, sizeof(tPlaylists*) * new_size);
    playlists = new;

    for (int m = new_size / 2; m < new_size; m++) {
        playlists[m] = AllocatePlaylist();
    }

    return playlists;
}

/**
 * @brief imprime as informações de todas as playlist existentes
 *
 * @param playlists
 * @param playlists_qty
 */
void DisplayPlaylists(tPlaylists** playlists, int playlists_qty) {
    // caso não tenha nenhuma playlist imprime o erro, caso contrário imprime as informações das playlists
    if (!playlists_qty) {
        RED_COLOUR;
        printf("• ERRO: Nenhuma playlist foi criada ainda.\n\n");
        NORMAL_COLOUR;
    } else {
        printf("• Informações sobre as playlists:\n\n");
        NORMAL_COLOUR;
        for (int m = 0; m < playlists_qty; m++) {
            printf("Nome: %s\n", playlists[m]->playlist_name);
            printf("Índice: %d\n", playlists[m]->index);
            printf("Músicas: %d\n\n", playlists[m]->tracks_qty);
        }
    }
}

/**
 * @brief procura playlist pelo index e imprime suas informações
 *
 * @param input index da playlist
 * @param playlists
 */
void SearchPlaylistByIndex(int input, tPlaylists** playlists) {
    printf("\nNome: %s\n", playlists[input]->playlist_name);
    ShowPlaylistTracks(playlists[input]->tracks, playlists[input]->tracks_qty);
}

/**
 * @brief adiciona música à playlist
 */
tPlaylists* LinkTrackToPlaylist(tPlaylists* playlist, tTracks* track) {
    // caso deseja colocar mais músicas do que tenha allocado, faz o realloc
    if (playlist->tracks_qty == *playlist->tracks_alloc_size) {
        *playlist->tracks_alloc_size *= 2;

        int* new_index = NULL;
        new_index = (int*)realloc(playlist->tracks_index, sizeof(int) * *playlist->tracks_alloc_size);
        playlist->tracks_index = new_index;
        playlist->tracks = ReallocateMorePlaylistsTracks(playlist->tracks, *playlist->tracks_alloc_size);
    }

    playlist->tracks_index[playlist->tracks_qty] = GetTrackIndex(track);
    playlist->tracks[playlist->tracks_qty] = track;

    playlist->tracks_qty++;

    TrackAddedToPlaylistCounter(track);

    return playlist;
}

char* GetPlaylistName(tPlaylists* playlist) {
    return playlist->playlist_name;
}

int GetPlaylistTracksQuantity(tPlaylists* playlist) {
    return playlist->tracks_qty;
}

/**
 * @brief compara música com uma playlist a partir da distância euclidiana
 * @param qty quantidade de músicas a serem comparadas
 */
void ComparePlaylistToTracks(tSpotfes* spotfes, tPlaylists* playlist, int qty) {
    int tracks_qty = GetTracksQuantity(spotfes);
    float aux, euclidean_distance[tracks_qty];

    GetAverages(playlist);

    for (int m = 0; m < tracks_qty; m++) {
        euclidean_distance[m] = CalculateEuclideanDistance(GetTracksFeatures(spotfes, m), playlist->averages);
    }

    RelatesDistanceToTrack(spotfes, euclidean_distance);

    // ordena distâncias euclidianas de modo crescente
    for (int m = 0; m < tracks_qty; m++) {
        for (int mm = m; mm < tracks_qty; mm++) {
            if (euclidean_distance[m] > euclidean_distance[mm]) {
                aux = euclidean_distance[m];
                euclidean_distance[m] = euclidean_distance[mm];
                euclidean_distance[mm] = aux;
            }
        }
    }

    // varre as músicas veficando as primeiras que tem distância euclidiana igual as menores distâncias calculadas, para a quantidade de músicas desejadas
    for (int m = 0; m < qty; m++) {
        for (int mm = 0; mm < tracks_qty; mm++) {
            float distance = GetTrackDistance(spotfes, mm);

            if (distance == euclidean_distance[m]) {
                if (!m) {
                    printf("\n");
                }
                PrintSimilarTrack(spotfes, mm);
                break;
            }
        }
    }
}

/**
 * @brief Calcula média de todas as características, necessárias para calcular a distância euclidiana, das músicas de uma playlist e guarda em um vetor
 */
void GetAverages(tPlaylists* playlist) {
    for (int m = 0; m < TRACK_STATS_QUANTITY; m++) {
        playlist->averages[m] = CalculateAverages(m, playlist->tracks, playlist->tracks_qty);
    }
}

/**
 * @brief escreve informações das playlists no arquivo binário
 *
 * @param playlists
 * @param quantity quantidade de playlists
 */
void WriteBinaryPlaylists(tPlaylists** playlists, int quantity) {
    FILE* playlists_file = fopen("binaries/playlists.bin", "wb");

    if (playlists_file == NULL) {
        PrintBinaryFilesError();
        exit(1);
    } else {
        fwrite(&quantity, sizeof(int), 1, playlists_file);

        for (int m = 0; m < quantity; m++) {
            fwrite(&playlists[m]->index, sizeof(int), 1, playlists_file);
            fwrite(&playlists[m]->name_size, sizeof(int), 1, playlists_file);
            fwrite(playlists[m]->playlist_name, sizeof(char), playlists[m]->name_size, playlists_file);
            fwrite(&playlists[m]->tracks_qty, sizeof(int), 1, playlists_file);
            fwrite(playlists[m]->tracks_index, sizeof(int), playlists[m]->tracks_qty, playlists_file);
            fwrite(playlists[m]->averages, sizeof(float), TRACK_STATS_QUANTITY, playlists_file);
            fwrite(playlists[m]->tracks_alloc_size, sizeof(int), 1, playlists_file);
        }

        fclose(playlists_file);
    }
}

/**
 * @brief libera um bloco de memória previamente alocado para as informações das playlists
 *
 * @param playlists
 */
void FreeUpPlaylists(tPlaylists* playlists) {
    FreeAndNullPointer(playlists->tracks_alloc_size);
    FreeAndNullPointer(playlists->tracks_index);
    FreeAndNullPointer(playlists->tracks);
    FreeAndNullPointer(playlists->playlist_name);
    FreeAndNullPointer(playlists->averages);
    FreeAndNullPointer(playlists);
}
