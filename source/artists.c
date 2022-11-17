#include "libraries.h"

/**
 * @brief struct usada para guardar informações sobre artistas
 */
struct tartists {
    char* id;
    int followers;
    char** genres;
    int genres_qty;
    char* artist_name;
    int popularity;
    int* times_added_to_playlist;  // em quantas playlists o artista aparece
};

tArtists* AllocateArtists() {
    return (tArtists*)calloc(sizeof(tArtists), 1);
}

/**
 * @brief lê arquivo com informações dos artistas
 * @return vetor de structs do tipo tArtists com as informações lidas
 */
tArtists** ReadArtistsDataFiles(tArtists** artists, FILE* artists_data, int* artists_qty) {
    char* buffer = (char*)malloc(sizeof(char) * DATA_BUFFER_SIZE);
    int alloc_size = STARTER_DATA_SIZE;

    // lê cada linha do arquivo por vez
    for (int m = 0; fgets(buffer, DATA_BUFFER_SIZE, artists_data) && !EndOfFile(buffer[0]); m++) {
        int line_size = strlen(buffer);

        *artists_qty = m + 1;
        if (*artists_qty > alloc_size) {
            alloc_size *= 2;
            artists = ReallocateMoreArtists(artists, alloc_size);
        }

        artists[m]->id = strdup(strtok(buffer, ";"));
        artists[m]->followers = atoi(strtok(NULL, ";"));

        char genres_line[line_size];
        strcpy(genres_line, strtok(NULL, ";"));

        artists[m]->artist_name = strdup(strtok(NULL, ";"));
        artists[m]->popularity = atoi(strtok(NULL, "\n"));

        ReadArtistsGenres(artists[m], genres_line);

        artists[m]->times_added_to_playlist = (int*)calloc(sizeof(int), 1);
        *artists[m]->times_added_to_playlist = 0;
    }

    FreeAndNullPointer(buffer);

    if (*artists_qty < alloc_size) {
        artists = ReallocateLessArtists(artists, alloc_size, *artists_qty);
    }

    return artists;
}

/**
 * @brief recebe uma string com os gêneros musicais concatenados e armazena cada um individualmente em um array contido nos artistas
 */
void ReadArtistsGenres(tArtists* artist, char* line) {
    int size = strlen(line);

    artist->genres_qty = GetValueQuantity(line, size);
    artist->genres = (char**)malloc(sizeof(char*) * artist->genres_qty);

    for (int mm = 0; mm < artist->genres_qty; mm++) {
        if (!mm) {
            artist->genres[mm] = strdup(strtok(line, "|"));
        } else {
            artist->genres[mm] = strdup(strtok(NULL, "|"));
        }
    }
}

/**
 * @brief realloca a struct de artistas dobrando a capacidade de armazenamento
 * @param new_size o tamanho do alloc anterior dobrado
 * @return um vetor contendo os artistas já lidos e os espaços para os novos artistas a serem lidos
 */
tArtists** ReallocateMoreArtists(tArtists** artists, int new_size) {
    tArtists** new = NULL;
    new = (tArtists**)realloc(artists, sizeof(tArtists*) * new_size);
    artists = new;

    for (int m = new_size / 2; m < new_size; m++) {
        artists[m] = AllocateArtists();
    }

    return artists;
}

/**
 * @brief realloca reduzindo a struct de artistas para a exata quantidade utilizada
 * @param old_size tamanho do alloc reservado à struct antes de entrar na função (maior)
 * @param new_size tamanho do alloc reservado à struct ao sair da função (menor)
 * @return o vetor de artistas sem o espaço de memória não utilizado
 */
tArtists** ReallocateLessArtists(tArtists** artists, int old_size, int new_size) {
    for (int m = new_size; m < old_size; m++) {
        FreeUpArtists(artists[m]);
    }

    tArtists** new = NULL;
    new = (tArtists**)realloc(artists, sizeof(tArtists*) * new_size);
    artists = new;

    return artists;
}

char* GetArtistID(tArtists* artist) {
    return artist->id;
}

/**
 * @brief lê no arquivo binário a quantidade de vezes que os artistas foram salvas em uma playlist
 * @param quantity quantidade de artistas
 */
void ReadBinaryArtists(tArtists** artists, int quantity) {
    FILE* playlists_file = fopen("binaries/artists.bin", "rb");
    size_t read = 0;

    if (playlists_file != NULL) {
        for (int m = 0; m < quantity; m++) {
            read = fread(artists[m]->times_added_to_playlist, sizeof(int), 1, playlists_file);
        }

        // se a função fread() não ler nada, ela não retornará nada para a variável 'read' que terá o valor zerado e encerrará o programa pelo if/else
        if (read) {
            fclose(playlists_file);
        } else {
            RED_COLOUR;
            printf("• ERRO: Leitura incompleta dos arquivos binários dos artistas.\n\n");
            NORMAL_COLOUR;
            exit(1);
        }
    }
}

/**
 * @brief imprime detalhes sobre os artistas de uma música específica
 */
void PrintTrackArtistsDetails(tArtists** artists, int artists_qty) {
    BLACK_COLOUR;
    if (artists_qty == 1) {
        printf("\n• Informações sobre a/o artista:\n\n");
    } else if (artists_qty > 1) {
        printf("\n• Informações sobre as/os artistas:\n\n");
    }

    NORMAL_COLOUR;
    for (int m = 0; m < artists_qty; m++) {
        // se o ponteiro pra um artista for nulo, significa que ele está em uma música, mas as informações sobre ele não estavam disponíveis no artists.csv
        // dessa forma, o argumento do laço for será incrementado em uma unidade, pois a variável "artists_qty" contabiliza somente artistas disponíveis no artists.csv
        if (artists[m] != NULL) {
            printf("Nome: %s\n", artists[m]->artist_name);
            printf("ID: %s\n", artists[m]->id);
            printf("Seguidores: %d\n", artists[m]->followers);
            printf("Popularidade: %d%%\n", artists[m]->popularity);

            if (artists[m]->genres_qty == 1) {
                printf("Gênero musical: ");
                if (artists[m]->genres[0][0] == '-') {
                    printf("não informado\n");
                } else {
                    printf("%s\n", artists[m]->genres[0]);
                }
            } else if (artists[m]->genres_qty > 1) {
                printf("Gêneros musicais: ");
                for (int mm = 0; mm < artists[m]->genres_qty; mm++) {
                    if (mm == artists[m]->genres_qty - 1) {
                        printf("%s\n", artists[m]->genres[mm]);
                    } else {
                        printf("%s, ", artists[m]->genres[mm]);
                    }
                }
            }
        } else {
            artists_qty++;
        }
    }
}

/**
 * @brief adiciona uma unidade toda vez que um determinado artista tiver uma música adicionada à uma playlist
 */
void ArtistsAddedToPlaylistCounter(tArtists* artist) {
    *artist->times_added_to_playlist += 1;
}

/**
 * @brief retorna a maior quantidade de vezes que um determinado artista teve uma música adicionada à uma playlist
 * @param artists todos os artistas lidos do .csv
 * @param qty a quantidade total de artistas
 */
int GetAddMostAddedArtist(tArtists** artists, int qty) {
    int time = 0;

    for (int m = 0; m < qty; m++) {
        if (time < *artists[m]->times_added_to_playlist) {
            time = *artists[m]->times_added_to_playlist;
        }
    }

    return time;
}

/**
 * @brief retorna a quantidade de vezes que um determinado artista teve uma música adicionada à uma playlist
 */
int GetArtistAddedCounter(tArtists* artist) {
    return *artist->times_added_to_playlist;
}

void PrintArtistName(FILE* artists_file, tArtists* artist) {
    fprintf(artists_file, "%s\n", artist->artist_name);
}

/**
 * @brief salva no arquivo binário a quantidade de vezes que os artistas foram salvas em uma playlist
 * @param quantity quantidade de artistas
 */
void WriteBinaryArtists(tArtists** artists, int quantity) {
    FILE* playlists_file = fopen("binaries/artists.bin", "wb");

    for (int m = 0; m < quantity; m++) {
        fwrite(artists[m]->times_added_to_playlist, sizeof(int), 1, playlists_file);
    }

    fclose(playlists_file);
}

/**
 * @brief libera um bloco de memória previamente alocado para as informações dos artistas
 */
void FreeUpArtists(tArtists* artists) {
    for (int m = 0; m < artists->genres_qty; m++) {
        FreeAndNullPointer(artists->genres[m]);
    }

    FreeAndNullPointer(artists->id);
    FreeAndNullPointer(artists->genres);
    FreeAndNullPointer(artists->artist_name);
    FreeAndNullPointer(artists->times_added_to_playlist);
    FreeAndNullPointer(artists);
}