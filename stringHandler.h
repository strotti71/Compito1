#ifndef STRING_HANDLER_H
#define STRING_HANDLER_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#define _MAX_LENGTH_WORD_ 31 // lunghezza massima della parola (30 char + \0)
#define MAX_RIGHE 1000000    // Numero massimo di righe del file csv
#define MAX_LEN_RIGA 10000   // numero massimo di caratteri nel csv file

typedef char type_parola[_MAX_LENGTH_WORD_];
typedef wchar_t type_parola_w[_MAX_LENGTH_WORD_];

int isSeparator(wchar_t c);
int isPunteggiatura(wchar_t c);
void appendCharToString(wchar_t *s, wchar_t c, int i);
void resettaParole(wchar_t *parolaPrecedente, wchar_t *parolaSuccessiva, int indexChar);
void pulisciStringa(wchar_t *s, int lunghezza);
void pulisciStringaChar(char *s, int lunghezza);
int is_special_char(const type_parola_w parola);

#endif