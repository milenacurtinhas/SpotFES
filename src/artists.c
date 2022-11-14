#include "libraries.h"

struct tartists {
    char* id;
    int followers;
    char** genres;
    int genres_qty;
    char* artist_name;
    int popularity;
    int* times_added_to_playlist;
};

tArtists* AllocateArtists() {
    return (tArtists*)calloc(sizeof(tArtists), 1);
}

tArtists** ReallocateMoreArtists(tArtists** artists, int new_size) {
    tArtists** new = NULL;
    new = (tArtists**)realloc(artists, sizeof(tArtists*) * new_size);
    artists = new;

    for (int m = new_size / 2; m < new_size; m++) {
        artists[m] = AllocateArtists();
    }

    return artists;
}

tArtists** ReallocateLessArtists(tArtists** artists, int old_size, int new_size) {
    for (int m = new_size; m < old_size; m++) {
        FreeUpArtists(artists[m]);
    }

    tArtists** new = NULL;
    new = (tArtists**)realloc(artists, sizeof(tArtists*) * new_size);
    artists = new;

    return artists;
}

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

tArtists** ReadArtistsDataFiles(tArtists** artists, FILE* artists_data, int* artists_qty) {
    char* buffer = (char*)malloc(sizeof(char) * 1024);
    int alloc_size = 128;

    for (int m = 0; fgets(buffer, 1024, artists_data) && !EndOfFile(buffer[0]); m++) {
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

void PrintTrackArtistsDetails(tArtists** artists, int artists_qty) {
    if (artists_qty == 1) {
        printf("\n• Informações sobre a/o artista:\n\n");
    } else if (artists_qty > 1) {
        printf("\n• Informações sobre as/os artistas:\n\n");
    }

    for (int m = 0; m < artists_qty; m++) {
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

char* GetArtistID(tArtists* artist) {
    return artist->id;
}

void ArtistsAddedToPlaylistCounter (tArtists* artist) {
    (*artist->times_added_to_playlist)++;
}