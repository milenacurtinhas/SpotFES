#include "libraries.h"

/**
 * @brief struct usada para guardar informações sobre artistas
 */
struct ttracks {
    int index;
    char* id;
    char* track_name;
    int popularity;
    int duration_ms;
    int explicit;
    char** track_artists;
    int artists_qty;
    char** artists_ids;
    int artists_ids_qty;
    int release_year;
    int release_month;
    int release_day;
    float danceability;
    float energy;
    int key;
    float loudness;
    int mode;
    float speechiness;
    float acousticness;
    float instrumentalness;
    float liveness;
    float valence;
    float tempo;
    int time_signature;
    tArtists** artists;
    int linked_artists_qty;
    float* features;               // vetor que guarda as oito características utilizadas para calcular a distância euclidiana
    float* euclidean_distance;     // guarda a distância euclidiana entre a música e a playlist a ser comparada
    int* times_added_to_playlist;  // guarda a quantidade de vezes em que a música é colocada em playlists
};

tTracks* AllocateTracks() {
    return (tTracks*)calloc(sizeof(tTracks), 1);
}

/**
 * @brief lê arquivo com informações das músicas
 * @return vetor de structs do tipo tTracks com as informações lidas
 */
tTracks** ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data, int* tracks_qty) {
    char* buffer = (char*)malloc(sizeof(char) * DATA_BUFFER_SIZE);
    int alloc_size = STARTER_DATA_SIZE;

    // lê cada linha do arquivo por vez
    for (int m = 0; fgets(buffer, DATA_BUFFER_SIZE, tracks_data) && !EndOfFile(buffer[0]); m++) {
        int line_size = strlen(buffer);
        *tracks_qty = m + 1;

        if (*tracks_qty > alloc_size) {
            alloc_size *= 2;
            tracks = ReallocateMoreTracks(tracks, alloc_size);
        }

        tracks[m]->index = m;
        tracks[m]->id = strdup(strtok(buffer, ";"));
        tracks[m]->track_name = strdup(strtok(NULL, ";"));
        tracks[m]->popularity = atoi(strtok(NULL, ";"));
        tracks[m]->duration_ms = atoi(strtok(NULL, ";"));
        tracks[m]->explicit = atoi(strtok(NULL, ";"));

        char artists_line[line_size];
        strcpy(artists_line, strtok(NULL, ";"));

        char artists_ids_line[line_size];
        strcpy(artists_ids_line, strtok(NULL, ";"));

        char release_date_line[line_size];
        strcpy(release_date_line, strtok(NULL, ";"));

        tracks[m]->danceability = atof(strtok(NULL, ";"));
        tracks[m]->energy = atof(strtok(NULL, ";"));
        tracks[m]->key = atoi(strtok(NULL, ";"));
        tracks[m]->loudness = atof(strtok(NULL, ";"));
        tracks[m]->mode = atoi(strtok(NULL, ";"));
        tracks[m]->speechiness = atof(strtok(NULL, ";"));
        tracks[m]->acousticness = atof(strtok(NULL, ";"));
        tracks[m]->instrumentalness = atof(strtok(NULL, ";"));
        tracks[m]->liveness = atof(strtok(NULL, ";"));
        tracks[m]->valence = atof(strtok(NULL, ";"));
        tracks[m]->tempo = atof(strtok(NULL, ";"));
        tracks[m]->time_signature = atoi(strtok(NULL, "\n"));
        tracks[m]->euclidean_distance = (float*)malloc(sizeof(float));

        ReadTrackArtists(tracks[m], artists_line);
        ReadTrackArtistsIDs(tracks[m], artists_ids_line);
        ReadTrackReleaseDate(tracks[m], release_date_line);
        PutFeaturesInArray(tracks[m]);

        tracks[m]->times_added_to_playlist = (int*)calloc(sizeof(int), 1);
    }

    FreeAndNullPointer(buffer);

    if (*tracks_qty < alloc_size) {
        tracks = ReallocateLessTracks(tracks, alloc_size, *tracks_qty);
    }

    return tracks;
}

/**
 * @brief lê a linha de artistas e separa cada artista
 */
void ReadTrackArtists(tTracks* track, char* line) {
    int size = strlen(line);

    track->artists_qty = GetValueQuantity(line, size);
    track->track_artists = (char**)malloc(sizeof(char*) * track->artists_qty);

    for (int mm = 0; mm < track->artists_qty; mm++) {
        if (!mm) {
            track->track_artists[mm] = strdup(strtok(line, "|"));
        } else {
            track->track_artists[mm] = strdup(strtok(NULL, "|"));
        }
    }
}

/**
 * @brief lê a linha de id de artistas e separa cada id
 */
void ReadTrackArtistsIDs(tTracks* track, char* line) {
    int size = strlen(line);

    track->artists_ids_qty = GetValueQuantity(line, size);
    track->artists_ids = (char**)malloc(sizeof(char*) * track->artists_ids_qty);

    for (int mm = 0; mm < track->artists_ids_qty; mm++) {
        if (!mm) {
            track->artists_ids[mm] = strdup(strtok(line, "|"));
        } else {
            track->artists_ids[mm] = strdup(strtok(NULL, "|"));
        }
    }
}

/**
 * @brief lê a linha de data e separa dia, mês e ano
 */
void ReadTrackReleaseDate(tTracks* track, char* line) {
    track->release_year = 0;
    track->release_month = 0;
    track->release_day = 0;

    int size = strlen(line);

    if (size >= YEAR) {
        track->release_year = atoi(strtok(line, "-"));
        if (size >= MONTH) {
            track->release_month = atoi(strtok(NULL, "-"));
            if (size == DAY) {
                track->release_day = atoi(strtok(NULL, "-"));
            }
        }
    }
}

/**
 * @brief guarda as oito características utilizadas para calcular a distância euclidiana no array
 */
void PutFeaturesInArray(tTracks* track) {
    track->features = (float*)malloc(sizeof(float) * TRACK_STATS_QUANTITY);

    track->features[0] = track->danceability;
    track->features[1] = track->energy;
    track->features[2] = track->mode;
    track->features[3] = track->speechiness;
    track->features[4] = track->acousticness;
    track->features[5] = track->instrumentalness;
    track->features[6] = track->liveness;
    track->features[7] = track->valence;
}

/**
 * @brief realloca a struct de músicas dobrando a capacidade de armazenamento
 * @param new_size o tamanho do alloc anterior dobrado
 * @return um vetor contendo os músicas já lidos e os espaços para os novos artistas a serem lidos
 */
tTracks** ReallocateMoreTracks(tTracks** tracks, int new_size) {
    tTracks** new = NULL;
    new = (tTracks**)realloc(tracks, sizeof(tTracks*) * new_size);
    tracks = new;

    for (int m = new_size / 2; m < new_size; m++) {
        tracks[m] = AllocateTracks();
    }

    return tracks;
}

/**
 * @brief realloca reduzindo a struct de músicas para a exata quantidade utilizada
 * @param old_size tamanho do alloc reservado à struct antes de entrar na função (maior)
 * @param new_size tamanho do alloc reservado à struct ao sair da função (menor)
 * @return o vetor de músicas sem o espaço de memória não utilizado
 */
tTracks** ReallocateLessTracks(tTracks** tracks, int old_size, int new_size) {
    for (int m = new_size; m < old_size; m++) {
        FreeUpTracks(tracks[m]);
    }

    tTracks** new = NULL;
    new = (tTracks**)realloc(tracks, sizeof(tTracks*) * new_size);
    tracks = new;

    return tracks;
}

/**
 * @brief adiciona o ponteiro de artista à música desse artista
 */
void LinkArtistsToTracks(tSpotfes* spotfes, tTracks** tracks, tArtists** artists) {
    int all_artists_qty = GetArtistsQuantity(spotfes);
    int tracks_qty = GetTracksQuantity(spotfes);
    char all_artists_ids[ID_BUFFER];

    // varre todas as tracks do spotfes
    for (int m = 0; m < tracks_qty; m++) {
        tracks[m]->artists = (tArtists**)malloc(sizeof(tArtists*) * tracks[m]->artists_ids_qty);
        tracks[m]->linked_artists_qty = 0;

        // varre todos os ids de artistas da track
        for (int mm = 0; mm < tracks[m]->artists_ids_qty; mm++) {
            // varre todos os artistas do spotfes
            for (int mmm = 0; mmm < all_artists_qty; mmm++) {
                strcpy(all_artists_ids, GetArtistID(artists[mmm]));

                // se a id do artista contida na track for igual à do artista encontrado, esse artista é adicionado à track
                if (strcmp(all_artists_ids, tracks[m]->artists_ids[mm]) == 0) {
                    tracks[m]->artists[mm] = artists[mmm];
                    tracks[m]->linked_artists_qty++;
                }

                // atribui null aos ponteiros quando o artista não está presente no vetor de artista do spotfes
                if (mmm == all_artists_qty - 1 && tracks[m]->linked_artists_qty == 0) {
                    tracks[m]->artists[mm] = NULL;
                }
            }
        }
    }
}

/**
 * @brief lê no arquivo binário a quantidade de vezes que as músicas foram salvas em uma playlist
 * @param quantity quantidade de músicas
 */
void ReadBinaryTracks(tTracks** tracks, int quantity) {
    FILE* playlists_file = fopen("binaries/tracks.bin", "rb");
    size_t read = 0;

    if (playlists_file != NULL) {
        for (int m = 0; m < quantity; m++) {
            read = fread(tracks[m]->times_added_to_playlist, sizeof(int), 1, playlists_file);
        }

        // se a função fread() não ler nada, ela não retornará nada para a variável 'read' que terá o valor zerado e encerrará o programa pelo if/else
        if (read) {
            fclose(playlists_file);
        } else {
            PrintBinaryFilesError();
            exit(1);
        }
    }
}

/**
 * @brief procura música pelo nome e imprime seus detalhes, caso encontrada
 * @param input nome da música a ser procurada
 */
void SearchTracksByTitle(char* input, tTracks** tracks, int tracks_qty) {
    int prints = 0;
    char* track = NULL;
    char* comparison = NULL;

    input = GetLowcaseString(input);

    for (int m = 0; m < tracks_qty; m++) {
        track = strdup(tracks[m]->track_name);
        track = GetLowcaseString(track);

        comparison = strstr(track, input);

        if (comparison != NULL) {
            prints++;

            PrintShortTracksDetails(tracks[m]);
            printf("ID: %s\n\n", tracks[m]->id);
        }

        FreeAndNullPointer(track);
    }

    if (prints == 0) {
        RED_COLOUR;
        printf("• ERRO: Nenhuma música foi encontrada!\n\n");
        NORMAL_COLOUR;
    }
}

/**
 * @brief imprime artista(s), título e índice da música
 */
void PrintShortTracksDetails(tTracks* track) {
    if (track->artists_qty == 1) {
        printf("Artista: ");
    } else {
        printf("Artistas: ");
    }

    PrintTrackArtists(track, track->artists_qty);
    printf("\nTítulo: %s\n", track->track_name);
    printf("Índice: %d\n", track->index);
}

/**
 * @brief imprime artista(s) da música
 * @param quantity quantidade de artistas a serem imprimidos
 */
void PrintTrackArtists(tTracks* track, int quantity) {
    if (quantity == 1) {
        printf("%s", track->track_artists[0]);
    } else {
        for (int m = 0; m < track->artists_qty; m++) {
            if (m == track->artists_qty - 1) {
                printf("%s", track->track_artists[m]);
            } else {
                printf("%s, ", track->track_artists[m]);
            }
        }
    }
}

/**
 * @brief procura música pelo index e imprime seus detalhes
 * @param input index da música a ser procurada
 */
void SearchTracksByIndex(int input, tTracks** tracks) {
    GREEN_COLOUR;
    printf("\n• Informações sobre a música:\n\n");
    NORMAL_COLOUR;
    printf("Título: %s\n", tracks[input]->track_name);
    printf("Índice: %d\n", tracks[input]->index);
    printf("ID: %s\n", tracks[input]->id);
    printf("Popularidade: %d%%\n", tracks[input]->popularity);
    PrintTrackLength(tracks[input]->duration_ms);
    PrintTrackReleaseDate(tracks[input]->release_day, tracks[input]->release_month, tracks[input]->release_year);
    printf("Danceability: %g\n", tracks[input]->danceability);
    printf("Energy: %g\n", tracks[input]->energy);
    printf("Key: %d\n", tracks[input]->key);
    printf("Loudness: %g\n", tracks[input]->loudness);
    printf("Mode: %d\n", tracks[input]->mode);
    printf("Speechiness: %g\n", tracks[input]->speechiness);
    printf("Acousticness: %g\n", tracks[input]->acousticness);
    printf("Instrumentalness: %g\n", tracks[input]->instrumentalness);
    printf("Liveness: %g\n", tracks[input]->liveness);
    printf("Valence: %g\n", tracks[input]->valence);
    printf("Tempo: %g\n", tracks[input]->tempo);
    printf("Time signature: %d\n", tracks[input]->time_signature);
    PrintTrackExplicitInfo(tracks[input]->explicit);
    PrintTrackArtistsDetails(tracks[input]->artists, tracks[input]->linked_artists_qty);
    OpenTrack(tracks[input]);
}

/**
 * @brief faz uma chamada de sistema para abrir a música do spotify no navegador
 */
void OpenTrack(tTracks* track) {
    char url[URL_BUFFER] = "firefox https://open.spotify.com/track/";
    strcat(url, track->id);

    GREEN_COLOUR;
    printf("\n♪ Deseja abrir a música no Firefox? (sim/não): ");

    if (GetValidYesOrNoInput()) {
        while (system(url) == 0) {
            break;
        }
    }

    NORMAL_COLOUR;
    printf("\n");
}

/**
 * @brief imprime músicas da playlist
 */
void ShowPlaylistTracks(tTracks** tracks_from_playlist, int tracks_qty) {
    if (!tracks_qty) {
        printf("Playlist vazia\n\n");
    } else {
        for (int m = 0; m < tracks_qty; m++) {
            if (!m) {
                printf("Músicas: %d\n\n", tracks_qty);
            }

            PrintShortTracksDetails(tracks_from_playlist[m]);
            printf("\n");
        }
    }
}

/**
 * @brief realloca o vetor de ponteiro de músicas que cabe na struct de playlist dobrando a capacidade de armazenamento
 * @param new_size o tamanho do alloc anterior dobrado
 * @return um vetor de ponteiro de músicas com o novo tamanho
 */
tTracks** ReallocateMorePlaylistsTracks(tTracks** tracks, int new_size) {
    tTracks** new = NULL;
    new = (tTracks**)realloc(tracks, sizeof(tTracks*) * new_size);
    tracks = new;

    return tracks;
}

int GetTrackIndex(tTracks* track) {
    return track->index;
}

/**
 * @brief o contador das vezes em que uma música e um artista foi adicionado em uma playlist é atualizado
 * @param track música do artista
 */
void TrackAddedToPlaylistCounter(tTracks* track) {
    *track->times_added_to_playlist += 1;

    for (int m = 0; m < track->artists_qty; m++) {
        ArtistsAddedToPlaylistCounter(track->artists[m]);
    }
}

char* GetTrackName(tTracks* track) {
    return track->track_name;
}

/**
 * @brief calcula média dos valores de uma característica das músicas de uma playlist
 * @param feature index da característica desejada
 * @param tracks músicas da playlist
 * @return média dos valores da característica
 */
float CalculateAverages(int feature, tTracks** tracks, int tracks_qty) {
    float sum = 0;

    for (int m = 0; m < tracks_qty; m++) {
        sum += GetFeatureValue(tracks[m], feature);
    }

    return sum / tracks_qty;
}

/**
 * @brief retorna valor de uma característica
 * @param track música da qual será retornada a característica
 * @param feature index da característica desejada
 * @return valor da característica
 */
float GetFeatureValue(tTracks* track, int feature) {
    switch (feature) {
        case 0:
            return track->danceability;
        case 1:
            return track->energy;
        case 2:
            return (float)track->mode;
        case 3:
            return track->speechiness;
        case 4:
            return track->acousticness;
        case 5:
            return track->instrumentalness;
        case 6:
            return track->liveness;
        case 7:
            return track->valence;
        default:
            exit(1);
    }
}

/**
 * @brief retorna o vetor que contém as características da música usadas para calcular a distância euclidiana
 * @return vetor que contém as características da música usadas para calcular a distância euclidiana
 */
float* GetFeatures(tTracks* tracks) {
    return tracks->features;
}

/**
 * @brief salva a distância euclidiana calculada entre a música e uma playlist na struct da música
 */
void SaveEuclideanDistanceToTrack(tTracks* track, float euclidean_distance) {
    *track->euclidean_distance = euclidean_distance;
}

/**
 * @brief retorna a distância euclidiana calculada da música com uma playlist
 * @return valor da distância euclidiana
 */
float GetDistance(tTracks* tracks) {
    return *tracks->euclidean_distance;
}

/**
 * @brief retorna a quantidade de vezes que a música mais adicionada em playlists foi adicionada nas playlists
 * @param qty quantidade de músicas
 * @return quantidade de vezes que a música mais adicionada em playlists foi adicionada nas playlists
 */
int GetAddMostAddedTrack(tTracks** tracks, int qty) {
    int time = 0;

    for (int m = 0; m < qty; m++) {
        if (time < *tracks[m]->times_added_to_playlist) {
            time = *tracks[m]->times_added_to_playlist;
        }
    }

    return time;
}

/**
 * @brief retorna a quantidade de vezes que uma música foi adicionada a uma playlist
 * @return quantidade de vezes que uma música foi adicionada a uma playlist
 */
int GetTracksAddedCounter(tTracks* track) {
    return *track->times_added_to_playlist;
}

void PrintTrackName(FILE* tracks_file, tTracks* track) {
    fprintf(tracks_file, "%s\n", track->track_name);
}

/**
 * @brief salva no arquivo binário a quantidade de vezes que as músicas foram salvas em uma playlist
 * @param quantity quantidade de músicas
 */
void WriteBinaryTracks(tTracks** tracks, int quantity) {
    FILE* playlists_file = fopen("binaries/tracks.bin", "wb");

    if (playlists_file == NULL) {
        PrintBinaryFilesError();
        exit(1);
    } else {
        for (int m = 0; m < quantity; m++) {
            fwrite(tracks[m]->times_added_to_playlist, sizeof(int), 1, playlists_file);
        }

        fclose(playlists_file);
    }
}

/**
 * @brief libera um bloco de memória previamente alocado para as informações dos músicas
 */
void FreeUpTracks(tTracks* tracks) {
    for (int m = 0; m < tracks->artists_qty; m++) {
        FreeAndNullPointer(tracks->track_artists[m]);
    }

    for (int m = 0; m < tracks->artists_ids_qty; m++) {
        FreeAndNullPointer(tracks->artists_ids[m]);
    }

    FreeAndNullPointer(tracks->id);
    FreeAndNullPointer(tracks->track_name);
    FreeAndNullPointer(tracks->track_artists);
    FreeAndNullPointer(tracks->artists_ids);
    FreeAndNullPointer(tracks->artists);
    FreeAndNullPointer(tracks->features);
    FreeAndNullPointer(tracks->euclidean_distance);
    FreeAndNullPointer(tracks->times_added_to_playlist);
    FreeAndNullPointer(tracks);
}