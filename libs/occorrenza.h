#ifndef OCCORRENZA_H
#define OCCORRENZA_H

#include "stringHandler.h"
#include <time.h>

int cercaParola(type_parola_w *a, type_parola_w parolaCercata, int lunghezzaArray);
void inserisciParola(type_parola_w *a, type_parola_w p, int lunghezzaArray);
double calcolaOccorrenze(int n, int nTot);
wchar_t *getParola(type_parola_w *arrayPaole, int index);
double generateRandomNum();
double wCharToDouble(wchar_t cWord[_MAX_LENGTH_WORD_]);
/*
struttura per registrare un'occorrenza di parola successiva.
 - parolaSuccessiva è un intero corrispondente alla cella dell'arrayParole
 - percentuale: double la percentuale di occorrenze per quella parola
*/
typedef struct
{
    int parolaSuccessiva;
    int n_Occorrenze;
} Occorrenza;

/*
Struttra per registrare i dati di ciascuna parola.

    - La parola è un intero corrispondente alla cella dell'arrayParole
    - il numero di parole successive trovate
    - un array di occorrenze delle parole trovate. (inizializzato a 0)
        ciascun occorrenza contiene :
            - la parola
            - la statistica di tale parola trovata nel testo
*/
typedef struct
{
    Occorrenza *occorrenze;
    int n_ParoleSuccessive;
    int totaleParoleSuccessive;
} Record;

typedef struct
{
    type_parola_w parola;
    double probability;
} ProbabilityOccorrenza;

typedef struct
{
    type_parola_w parola;
    ProbabilityOccorrenza *probabilityOccorrenze;
    int numeroParoleSuccessive;
} ProbabilityRecord;
// int cercaIntero(Record *rec, int numeroCercato, int lunghezzaArray);

int cercaParolaArrayProbability(ProbabilityRecord *probRec, type_parola_w parolaCercata, int len);

#endif