#ifndef AUDISEN2024_AMP_H
#define AUDISEN2024_AMP_H

#include "define.h"


FILE* initAMP(char* filename);

char* readAMP(FILE* pf);

void closeAMP(FILE* pf);

#endif //AUDISEN2024_AMP_H