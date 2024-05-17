#ifndef PARAMETER_H
#define PARAMETER_H

#include <stdio.h>
#include "stringHandler.h"
// #include "occorrenza.h"

int apriFile(char *filename, FILE *f);
int esportaCsv_old(type_parola *source, char *fileName);
int contaCaratteri(FILE *f);
// int esportaCsv(Record *source, int len, char *fileName);

#endif