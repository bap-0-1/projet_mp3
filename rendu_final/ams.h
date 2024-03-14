#ifndef AUDISEN2024_AMS_H
#define AUDISEN2024_AMS_H

#include "define.h"


#ifdef _WIN32
size_t getline(char **lineptr, size_t *n, FILE *stream);
#endif

typedef struct {
    char symbole[333];
    int note;
} notes;

struct song initSong();
char* replace_char(char* str, char find, char replace);
struct song readAMS(char* fileName);
int findNoteFromSymbol(char* symbol);
void createAMS(char* txtFileName, char* amsFileName);


#endif //AUDISEN2024
