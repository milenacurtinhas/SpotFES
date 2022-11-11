#include "libraries.h"

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
    float* features;
};

tTracks* AllocateTracks() {
    return (tTracks*)malloc(sizeof(tTracks));
}

tTracks** ReallocateMoreTracks(tTracks** tracks, int new_size) {
    tTracks** new = NULL;
    new = (tTracks**)realloc(tracks, sizeof(tTracks*) * new_size);
    tracks = new;

    for (int m = new_size / 2; m < new_size; m++) {
        tracks[m] = AllocateTracks();
    }

    return tracks;
}

tTracks** ReallocateLessTracks(tTracks** tracks, int old_size, int* new_size) {
    for (int m = (*new_size); m < old_size; m++) {
        FreeUpTracks(tracks[m]);
    }

    tTracks** new = NULL;
    new = (tTracks**)realloc(tracks, sizeof(tTracks*) * *new_size);
    tracks = new;

    return tracks;
}

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
    FreeAndNullPointer(tracks);
}

tTracks** ReadTracksDataFiles(tTracks** tracks, FILE* tracks_data, int* tracks_qty) {
    char* buffer = (char*)malloc(sizeof(char) * 2048);
    int alloc_size = 128, line_size, artists_line_size, artists_ids_line_size, release_date_line_size;

    for (int m = 0; fgets(buffer, 2048, tracks_data) && !EndOfFile(buffer[0]); m++) {
        line_size = strlen(buffer);
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

        artists_line_size = strlen(artists_line);
        tracks[m]->artists_qty = GetValueQuantity(artists_line, artists_line_size);
        tracks[m]->track_artists = (char**)malloc(sizeof(char*) * tracks[m]->artists_qty);

        artists_ids_line_size = strlen(artists_ids_line);
        tracks[m]->artists_ids_qty = GetValueQuantity(artists_ids_line, artists_ids_line_size);
        tracks[m]->artists_ids = (char**)malloc(sizeof(char*) * tracks[m]->artists_ids_qty);

        for (int mm = 0; mm < tracks[m]->artists_qty; mm++) {
            if (!mm) {
                tracks[m]->track_artists[mm] = strdup(strtok(artists_line, "|"));
            } else if (mm == tracks[m]->artists_qty - 1) {
                tracks[m]->track_artists[mm] = strdup(strtok(NULL, "\n"));
            } else {
                tracks[m]->track_artists[mm] = strdup(strtok(NULL, "|"));
            }
        }

        for (int mm = 0; mm < tracks[m]->artists_ids_qty; mm++) {
            if (!mm) {
                tracks[m]->artists_ids[mm] = strdup(strtok(artists_ids_line, "|"));
            } else if (mm == tracks[m]->artists_qty - 1) {
                tracks[m]->artists_ids[mm] = strdup(strtok(NULL, "\n"));
            } else {
                tracks[m]->artists_ids[mm] = strdup(strtok(NULL, "|"));
            }
        }

        tracks[m]->release_year = 0;
        tracks[m]->release_month = 0;
        tracks[m]->release_day = 0;

        release_date_line_size = strlen(release_date_line);

        if (release_date_line_size >= 4) {
            tracks[m]->release_year = atoi(strtok(release_date_line, "-"));
        }

        if (release_date_line_size >= 7) {
            tracks[m]->release_month = atoi(strtok(NULL, "-"));
        }

        if (release_date_line_size == 10) {
            tracks[m]->release_day = atoi(strtok(NULL, "-"));
        }

        PutFeaturesInArray(tracks[m]);
    }

    FreeAndNullPointer(buffer);

    if (*tracks_qty < alloc_size) {
        tracks = ReallocateLessTracks(tracks, alloc_size, tracks_qty);
    }

    return tracks;
}

void LinkArtistsToTracks(tSpotfes* spotfes, tTracks** tracks, tArtists** artists) {
    int all_artists_qty = GetArtistsQuantity(spotfes);
    int tracks_qty = GetTracksQuantity(spotfes);
    char all_artists_ids[23];
    // varre todas as tracks do spotfes
    for (int m = 0; m < tracks_qty; m++) {
        tracks[m]->artists = (tArtists**)malloc(sizeof(tArtists*) * tracks[m]->artists_ids_qty);
        tracks[m]->linked_artists_qty = 0;

        // varre todos os ids de artistas da track
        for (int mm = 0; mm < tracks[m]->artists_ids_qty; mm++) {
            // varre todos os artistas do spotfes
            for (int mmm = 0; mmm < all_artists_qty; mmm++) {
                strcpy(all_artists_ids, GetArtistID(artists[mmm]));

                if (strcmp(all_artists_ids, tracks[m]->artists_ids[mm]) == 0) {
                    tracks[m]->artists[mm] = artists[mmm];
                    tracks[m]->linked_artists_qty++;
                }

                if (mmm == all_artists_qty - 1 && tracks[m]->linked_artists_qty == 0) {
                    tracks[m]->artists[mm] = NULL;
                }
            }
        }
    }
}

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

            printf("\nÍndice da música: %d\n", tracks[m]->index);
            printf("ID da música: %s\n", tracks[m]->id);
            printf("Título da música: %s\n", tracks[m]->track_name);

            if (tracks[m]->artists_qty == 1) {
                printf("Artista da música: %s\n\n", tracks[m]->track_artists[0]);
            } else {
                printf("Artistas da música: ");
                for (int mm = 0; mm < tracks[m]->artists_qty; mm++) {
                    if (mm == tracks[m]->artists_qty - 1) {
                        printf("%s\n\n", tracks[m]->track_artists[mm]);
                    } else {
                        printf("%s, ", tracks[m]->track_artists[mm]);
                    }
                }
            }
        }

        FreeAndNullPointer(track);
    }

    if (prints == 0) {
        printf("\nNenhuma música foi encontrada!\n\n");
    }
}

void SearchTracksByIndex(int input, tTracks** tracks) {
    printf("\n♪  Informações sobre a música  ♪\n");
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

void OpenTrack(tTracks* track) {
    char option;
    char url[62] = "firefox https://open.spotify.com/track/";
    strcat(url, track->id);

    printf("\n♪  Deseja abrir a música no Firefox? (S/N): ");
    scanf("%c%*c", &option);

    if (option == 'S' || option == 's') {
        system(url);
    } else if (option != 'N' && option != 'n') {
        printf("\nERRO: Opção inválida. Reinicie o programa.\n");
        exit(1);
    }

    printf("\n");
}

void DisplayTracks(tTracks** tracks_from_playlist, int tracks_qty) {
    printf("Músicas: ");
    for (int m = 0; m < tracks_qty; m++) {
        printf("%s", tracks_from_playlist[m]->track_name);
        if (m != tracks_qty - 1) {
            printf(" • ");
        }
    }
    printf("\n");
}

/*float CalculateAverages(int feature, tTracks** tracks, int tracks_qty) {
    float sum = 0;
    for (int m = 0; m < tracks_qty; m++) {
        sum += GetFeatureValue(tracks[m], feature);
    }
    float average = sum / tracks_qty;
    return average;
}

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
}*/

void PutFeaturesInArray(tTracks* track) {
    track->features = (float*)malloc(sizeof(float) * 8);

    track->features[0] = track->danceability;
    track->features[1] = track->energy;
    track->features[2] = track->mode;
    track->features[3] = track->speechiness;
    track->features[4] = track->acousticness;
    track->features[5] = track->instrumentalness;
    track->features[6] = track->liveness;
    track->features[7] = track->valence;
}