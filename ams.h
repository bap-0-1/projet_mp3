#ifndef AUDISEN2024_AMS_H
#define AUDISEN2024_AMS_H

#include "define.h"

typedef struct {
    char symbole[333];
    int note;
} notes;

notes convertSymbolesNotes[] = {{"C1", 1}, {"C#1", 2}, {"D1", 3}, {"D#1", 4}, {"E1", 5}, {"F1", 6}, {"F#1", 7}, {"G1", 8}, {"G#1", 9}, {"A1", 10}, {"A#1", 11}, {"B1", 12}, {"C2", 13}, {"C#2", 14}, {"D2", 15}, {"D#2", 16}, {"E2", 17}, {"F2", 18}, {"F#2", 19}, {"G2", 20}, {"G#2", 21}, {"A2", 22}, {"A#2", 23}, {"B2", 24}, {"C3", 25}, {"C#3", 26}, {"D3", 27}, {"D#3", 28}, {"E3", 29}, {"F3", 30}, {"F#3", 31}, {"G3", 32}, {"G#3", 33}, {"A3", 34}, {"A#3", 35}, {"B3", 36}, {"C4", 37}, {"C#4", 38}, {"D4", 39}, {"D#4", 40}, {"E4", 41}, {"F4", 42}, {"F#4", 43}, {"G4", 44}, {"G#4", 45}, {"A4", 46}, {"A#4", 47}, {"B4", 48}, {"C5", 49}, {"C#5", 50}, {"D5", 51}, {"D#5", 52}, {"E5", 53}, {"F5", 54}, {"F#5", 55}, {"G5", 56}, {"G#5", 57}, {"A5", 58}, {"A#5", 59}, {"B5", 60}};

struct song initSong();
struct song readAMS(char* fileName);
int findNoteFromSymbol(char* symbol);
void createAMS(char* txtFileName, char* amsFileName);


#endif //AUDISEN2024
