#ifndef LIBRARIES_H
#define LIBRARIES_H

#define DATA_BUFFER_SIZE 2048
#define STARTER_DATA_SIZE 128
#define STARTER_PLAYLISTS_SIZE 16
#define TRACK_STATS_QUANTITY 8

#define STRING_BUFFER_SIZE 128
#define URL_BUFFER 64
#define ID_BUFFER 32

#define NORMAL_COLOUR printf("\033[0m");
#define BLACK_COLOUR printf("\033[1;30m");
#define RED_COLOUR printf("\033[1;31m");
#define GREEN_COLOUR printf("\033[1;32m");

#define DAY 10
#define MONTH 7
#define YEAR 4

#define ONLINE 1

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

#endif