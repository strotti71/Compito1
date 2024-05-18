#ifndef PARAMETER_H
#define PARAMETER_H

typedef struct
{
    char inputFileName[30];
    int nParoleDaGenerare;
    char startingWord[30];
} ParametriInput;

void leggiParametri(int argc, char *argv[], ParametriInput *parametri);
#endif