#include "libraries.h"

struct tspotfes {
    tArtists** artists;
    int artists_qty;
    int artists_mallocs;
    tTracks** tracks;
    int tracks_qty;
    int tracks_mallocs;
    tPlaylists** playlists;
    int playlists_qty;
    int playlists_mallocs;
};

void CheckDataFilesPath(int argc) {
    switch (argc) {
        case 1:
            printf("ERRO: Nenhum arquivo de dados do Spotify foi declarado. Verifique se eles estão contidos na pasta 'data', no mesmo diretório desse programa.\n");
            exit(1);
            break;
        case 2:
            printf("ERRO: Um dos arquivos de dados do Spotify não foi declarado. Verifique se ele está contido na pasta 'data', no mesmo diretório desse programa.\n");
            exit(1);
            break;
        case 3:
            break;
    }
}

tSpotfes* AllocateSpotfes() {
    tSpotfes* spotfes = (tSpotfes*)malloc(sizeof(tSpotfes));
    spotfes->artists = (tArtists**)malloc(sizeof(tArtists*) * 128);
    spotfes->tracks = (tTracks**)malloc(sizeof(tTracks*) * 128);
    spotfes->playlists = (tPlaylists**)malloc(sizeof(tPlaylists*) * 16);

    for (int m = 0; m < 128; m++) {
        spotfes->artists[m] = AllocateArtists();
        spotfes->tracks[m] = AllocateTracks();
        if (m < 16) {
            spotfes->playlists[m] = AllocatePlaylists();
        }
    }

    spotfes->artists_qty = 0;
    spotfes->artists_mallocs = 128;
    spotfes->tracks_qty = 0;
    spotfes->tracks_mallocs = 128;
    spotfes->playlists_qty = 0;
    spotfes->playlists_mallocs = 16;

    return spotfes;
}

void FreeUpSpotfes(tSpotfes* spotfes) {
    for (int m = 0; m < spotfes->artists_mallocs; m++) {
        FreeUpArtists(spotfes->artists[m]);
    }

    for (int m = 0; m < spotfes->tracks_mallocs; m++) {
        FreeUpTracks(spotfes->tracks[m]);
    }

    for (int m = 0; m < spotfes->playlists_mallocs; m++) {
        FreeUpPlaylists(spotfes->playlists[m]);
    }

    FreeAndNullPointer(spotfes->artists);
    FreeAndNullPointer(spotfes->tracks);
    FreeAndNullPointer(spotfes->playlists);
    FreeAndNullPointer(spotfes);
}

void ReadSpotifyDataFiles(tSpotfes* spotfes, char** argv) {
    FILE* artists_data = fopen(argv[1], "r");
    if (!artists_data) {
        printf("ERRO: Não foi possível acessar o arquivo de dados dos artistas. Verifique se ele está no mesmo diretório do executável desse programa.\n");
        exit(1);
    }

    spotfes->artists_qty = ReadArtistsDataFiles(spotfes->artists, artists_data);
    fclose(artists_data);

    FILE* tracks_data = fopen(argv[2], "r");
    if (!tracks_data) {
        printf("ERRO: Não foi possível acessar o arquivo de dados das músicas. Verifique se ele está no mesmo diretório do executável desse programa.\n");
        exit(1);
    }

    spotfes->tracks_qty = ReadTracksDataFiles(spotfes->tracks, tracks_data);
    fclose(tracks_data);
}

int SetUpMainMenu(int input) {
    printf("SPOTFES by M&M\n\n");
    printf("1 - Buscar músicas\n");
    printf("2 - Listar uma música\n");
    printf("3 - Criar uma playlist\n");
    printf("4 - Listar playlists\n");
    printf("5 - Listar uma playlist\n");
    printf("6 - Adicionar uma música na playlist\n");
    printf("7 - Recomendar músicas parecidas com uma playlist\n");
    printf("8 - Gerar relatório\n\n");
    printf("Digite a opção desejada: ");
    scanf("%d%*c", &input);
    
    return input;
}