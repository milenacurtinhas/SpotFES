#include "libraries.h"

struct tartists {
    char* id;
    int followers;
    char** genres;
    int genres_qty;
    char* artist_name;
    int popularity;
};

tArtists* AllocateArtists() {
    return (tArtists*)malloc(sizeof(tArtists));
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

tArtists** ReallocateLessArtists(tArtists** artists, int old_size, int* new_size) {
    for (int m = (*new_size); m < old_size; m++) {
        FreeUpArtists(artists[m]);
    }

    tArtists** new = NULL;
    new = (tArtists**)realloc(artists, sizeof(tArtists*) * *new_size);
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
    FreeAndNullPointer(artists);
}

tArtists** ReadArtistsDataFiles(tArtists** artists, FILE* artists_data, int* artists_qty) {
    char* buffer = (char*)malloc(sizeof(char) * 1024);
    int alloc_size = 128, line_size, genres_line_size;

    for (int m = 0; fgets(buffer, 1024, artists_data) && !EndOfFile(buffer[0]); m++) {
        line_size = strlen(buffer);

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

        genres_line_size = strlen(genres_line);
        artists[m]->genres_qty = GetValueQuantity(genres_line, genres_line_size);
        artists[m]->genres = (char**)malloc(sizeof(char*) * artists[m]->genres_qty);

        for (int mm = 0; mm < artists[m]->genres_qty; mm++) {
            if (!mm) {
                artists[m]->genres[mm] = strdup(strtok(genres_line, "|"));
            } else if (mm == artists[m]->genres_qty - 1) {
                artists[m]->genres[mm] = strdup(strtok(NULL, "\n"));
            } else {
                artists[m]->genres[mm] = strdup(strtok(NULL, "|"));
            }
        }
    }

    FreeAndNullPointer(buffer);

    if (*artists_qty < alloc_size) {
        artists = ReallocateLessArtists(artists, alloc_size, artists_qty);
    }

    return artists;
}

char* GetArtistID(tArtists* artist) {
    return artist->id;
}