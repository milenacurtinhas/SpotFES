#include "libraries.h"

struct tartists {
    char id[23];
    int followers;
    char** genres;
    int genres_qty;
    char* artist_name;
    int popularity;
};

tArtists* AllocateArtists() {
    return (tArtists*)malloc(sizeof(tArtists));
}

void FreeUpArtists(tArtists* artists) {
    for (int m = 0; m < artists->genres_qty; m++) {
        FreeAndNullPointer(artists->genres[m]);
    }
    FreeAndNullPointer(artists->genres);
    FreeAndNullPointer(artists->artist_name);
    FreeAndNullPointer(artists);
}

int ReadArtistsDataFiles(tArtists** artists, FILE* artists_data) {
    char buffer[1024];
    int artists_qty = 1, line_size, genres_line_size, name_size;

    for (int m = 0; fgets(buffer, 1024, artists_data) && !EndOfFile(buffer[0]); m++) {
        line_size = strlen(buffer);
        artists_qty = m + 1;

        strcpy(artists[m]->id, strtok(buffer, ";"));

        artists[m]->followers = atoi(strtok(NULL, ";"));

        char genres_line[line_size];
        strcpy(genres_line, strtok(NULL, ";"));

        char name[line_size];
        strcpy(name, strtok(NULL, ";"));
        name_size = strlen(name);
        artists[m]->artist_name = (char*)malloc(sizeof(name_size + 1));
        strcpy(artists[m]->artist_name, name);

        artists[m]->popularity = atoi(strtok(NULL, "\n"));

        genres_line_size = strlen(genres_line);
        artists[m]->genres_qty = GetGenresQuantity(genres_line, genres_line_size);
        artists[m]->genres = (char**)malloc(sizeof(char*) * artists[m]->genres_qty);

        for (int mm = 0; mm < artists[m]->genres_qty; mm++) {
            if (!mm) {
                strcpy(genres_line, (strtok(genres_line, "|")));
            } else if (mm == artists[m]->genres_qty - 1) {
                strcpy(genres_line, (strtok(NULL, "\n")));
            } else {
                strcpy(genres_line, (strtok(NULL, "|")));
            }
            genres_line_size = strlen(genres_line);
            artists[m]->genres[mm] = (char*)malloc(sizeof(genres_line_size + 1));
            strcpy(artists[m]->genres[mm], genres_line);
        }
    }
    /*
        for (int m = 0; m < artists_qty; m++) {
            printf("\nID: %s\n", artists[m]->id);
            printf("Followers: %d\n", artists[m]->followers);
            for (int mm = 0; mm < artists[m]->genres_qty; mm++) {
                printf("Genre %d: %s\n", mm + 1, artists[m]->genres[mm]);
            }
            printf("Name: %s\n", artists[m]->artist_name);
            printf("Popularity: %d\n\n", artists[m]->popularity);
        }
    */
    return artists_qty;
}