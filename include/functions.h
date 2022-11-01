#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void FreeAndNullPointer(void* pointer);
int EndOfFile(char character);
int GetValueQuantity(char* character, int value_size);
int LessArtistsThanMallocs(int quantity, int mallocs);
int SameID(char* source_id, char* target_id);

#endif