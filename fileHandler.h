#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "stringHandler.h"
#include "occorrenza.h"

// void leggiFile(const char *nomeFile, int *numRighe);
int contaCaratteri(FILE *f);
FILE *apriFile(char *filename, char *modalita);
int esportaCsv(type_parola *paroleDistinte, Record *arrayRecParole, int len, char *fileName);
int contarighe(FILE *f);
#endif