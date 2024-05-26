#include <wchar.h>
#include <wctype.h>
#include "../libs/fileHandler.h"
#include "../libs/occorrenza.h"
#include "../libs/stringHandler.h"
#include "../libs/parameterHandler.h"

#define SYZE_CHAR sizeof(char)
#define SYZE_RECORD sizeof(Record)
#define _MAX_LENGTH_WORD_ 31 // lunghezza massima della parola (30 char + \0)

type_parola_w *arrayParoleDistinte; // Array contiene tutte le singole istanze delle parole
Record arrayRecordParole[1000000];
ProbabilityRecord *probabilityRecord; // Array in cui vengono caricate le probabilità per ciascuna parola
int unesedVArM;

/********************************
 *                              *
 *          VARIABILI           *
 *                              *
 ********************************/
FILE *fin;
wchar_t *fileNormalizzato;     // file riportato su array con eliminazione dei doppi spazi e separazione di tutti i caratteri di punteggiatura
int lenFileNormalizzato = 0;   // lunghezza del file normalizzato
int n_CharFileNormalizzato;    // numero caratteri del file normalizzato
int n_DistinctParoleTesto = 0; // numero di parole distinte nel testo. ciascuna parola viene caricata nell'array parole
int n_ParoleTotali = 0;
int numeroParole;
ParametriInput parametri;

//*******************************
//                              *
//          FUNZIONI            *
//                              *
//*******************************

void init(int args, char *argv[], ParametriInput *parametri);
void produciTabellaOccorrenze(int pipe_fd[2]);
void generaTesto();
void preparaStream(FILE *fin, wchar_t *out);
void inserisciCarattere(wchar_t **arr, wchar_t c, int *n_CharFileNormalizzato);
void popolaArrayParoleDistinte(wchar_t *fin);
void popolaArrayRecordOccorrenze(wchar_t *fin);
void creaArrayProb();
