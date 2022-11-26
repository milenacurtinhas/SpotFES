#ifndef LIBRARIES_H
#define LIBRARIES_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artists.h"
#include "functions.h"
#include "spotfes.h"
#include "tracks.h"
#include "playlists.h"

// o valor inicial de allocs para as estruturas dos artistas e das músicas
#define STARTER_DATA_SIZE 128
// o valor inicial de allocs para as estruturas das playlists
#define STARTER_PLAYLISTS_SIZE 16
// o número de características utilizadas no algoritmo de recomendação de músicas semelhantes
#define TRACK_STATS_QUANTITY 8

// o valor padrão para armazenar a string da leitura de cada linha dos arquivos .csv
#define DATA_BUFFER_SIZE 2048
// o valor padrão para armazenar strings
#define STRING_BUFFER_SIZE 128
// o valor padrão para armazenar a string da url da música
#define URL_BUFFER 64
// o valor padrão para armazenar a string dos ids das músicas e dos artistas
#define ID_BUFFER 32

// retorna a cor do texto para o padrão
#define NORMAL_COLOUR printf("\033[0m");
// altera a cor do texto para vermelho
#define RED_COLOUR printf("\033[1;31m");
// altera a cor do texto para verde
#define GREEN_COLOUR printf("\033[1;32m");

// MELHORA A EXIBIÇÃO DE VALORES EM CONDIÇÕES IF/ELSE
// se a string de data possuir ano, mês e dia, o tamanho dela será de 10 caracteres
#define DAY 10
// se a string de data possuir somente o ano e o mês, o tamanho dela será de 7 caracteres
#define MONTH 7
// se a string de data possuir somente o ano, o tamanho dela será de 4 caracteres
#define YEAR 4

// melhora a exibição de valores booleanos em laços de repetição
#define ONLINE 1

#endif