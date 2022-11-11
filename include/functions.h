#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void FreeAndNullPointer(void* pointer);
int EndOfFile(char character);
int GetValueQuantity(char* character, int value_size);
char* GetLowcaseString(char* string);
void PrintTrackLength(int duration_ms);
void PrintTrackReleaseDate(int day, int month, int year);
void PrintTrackExplicitInfo(int explicit);
float CalculateEuclideanDistance(float* features, float* averages);

#endif