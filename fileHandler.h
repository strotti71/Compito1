#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "stringHandler.h"
// #include "occorrenza.h"
char **leggiFile(const char *nomeFile, int *numRighe);
int contaCaratteri(FILE *f);
FILE *apriFile(char *filename, char *modalita);
int esportaCsv_old(type_parola *source, char *fileName);

// int esportaCsv(Record *source, int len, char *fileName);

#endif