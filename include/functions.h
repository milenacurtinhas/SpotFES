#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void CheckDataFilesPath(int argc, char** argv);
void ClearTerminal();
void PrintMissingFilesErrorAndQuitProgram();
int EndOfFile(char character);
int GetValueQuantity(char* character, int value_size);
void FreeAndNullPointer(void* pointer);
void PrintMenuArt();
int GetValidIntegerInput(int min_range, int max_range);
char GetValidYesOrNoInput();
char* GetLowcaseString(char* string);
void PrintTrackLength(int duration_ms);
void PrintTrackReleaseDate(int day, int month, int year);
void PrintTrackExplicitInfo(int explicit);
float CalculateEuclideanDistance(float* features, float* averages);

#endif