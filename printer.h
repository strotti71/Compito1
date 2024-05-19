#ifndef PRINTER_H
#define PRINTER_H
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "stringHandler.h"
#include "parameterHandler.h"

void stampaArrayParole(type_parola_w *arr, int len, char *s);
void stampaArrayCaratteri(const wchar_t *arr, int len, char *s);
void stampaParametri(ParametriInput *parametri);
void stampaRiga(char *riga);
#endif