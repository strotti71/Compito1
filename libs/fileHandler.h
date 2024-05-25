#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include "stringHandler.h"
#include "occorrenza.h"

#define OUTPUT_FILE "OUTPUT"  // file di testo generato
#define CSV_FILE "export.csv" // file csv delle occorrenze

FILE *apriFile(char *filename, char *modalita);
int contaCaratteri(FILE *f);
int esportaCsv(type_parola_w *paroleDistinte, Record *arrayRecParole, int len, char *fileName);
int contarighe(FILE *f);
void stampa_file(const char *file_path);
#endif