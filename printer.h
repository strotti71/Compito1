#ifndef PRINTER_H
#define PRINTER_H

#include "stringHandler.h"
#include "parameterHandler.h"

void stampaArrayParole(type_parola *arr, int len, char *s);
void stampaArrayCaratteri(char *arr, int len, char *s);
void stampaParametri(ParametriInput *parametri);
#endif