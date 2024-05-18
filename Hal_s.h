#include "fileHandler.h"
#include "occorrenza.h"
#include "stringHandler.h"
#include "parameterHandler.h"

#define SYZE_CHAR sizeof(char)
#define SYZE_RECORD sizeof(Record)
#define _MAX_LENGTH_WORD_ 31 // lunghezza massima della parola (30 char + \0)

type_parola *arrayParoleDistinte; // Array contiene tutte le singole istanze delle parole
Record arrayRecordParole[1000000];
ProbabilityRecord *probabilityRecord; // Array in cui vengono caricate le probabilità per ciascuna parola

/********************************
 *                              *
 *          VARIABILI           *
 *                              *
 ********************************/
FILE *fin;
char *fileNormalizzato;        // file riportato su array con eliminazione dei doppi spazi e separazione di tutti i caratteri di punteggiatura
int lenFileNormalizzato = 0;   // lunghezza del file normalizzato
int n_CharFileNormalizzato;    // numero caratteri del file normalizzato
int n_DistinctParoleTesto = 0; // numero di parole distinte nel testo. ciascuna parola viene caricata nell'array parole
int n_ParoleTotali = 0;
int numeroParole;

//*******************************
//                              *
//          FUNZIONI            *
//                              *
//*******************************
int contarighe(FILE *f);
void init(int args, char *argv[], ParametriInput *parametri);
void generaTesto();
double generateRandomNum();
void preparaStream(FILE *fin, char *out);
void inserisciCarattere(char *arr, char c, int *n_CharFileNormalizzato);
void popolaArrayParoleDistinte(char *fin);
void popolaArrayRecordOccorrenze(char *fin);
void creaArrayProb();
void stampaRiga(char *riga);
double calcolaOccorrenze(int n, int nTot);
char *getParola(type_parola *arrayPaole, int index);