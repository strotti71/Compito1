#ifndef PARAMETER_H
#define PARAMETER_H
#include "stringHandler.h"
typedef struct
{
    char inputFileName[_MAX_LENGTH_WORD_];
    int nParoleDaGenerare;
    char startingWord[_MAX_LENGTH_WORD_];
} ParametriInput;

void leggiParametri(int argc, char *argv[], ParametriInput *parametri);
#endif