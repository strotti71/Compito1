#define SYZE_CHAR sizeof(char)
#define SYZE_RECORD sizeof(Record)
#define _MAX_LENGTH_WORD_ 31 // lunghezza massima della parola (30 char + \0)

typedef char type_parola[_MAX_LENGTH_WORD_];
int nCharInput; // numero caratteri nel file di input

type_parola arrayParoleDistinte[10000]; // Array contiene tutte le singole istanze delle parole
int n_DistinctParoleTesto = 0;          // numero di parole distinte nel testo. ciascuna parola viene caricata nell'array parole
int n_ParoleTotali = 0;

char *fileNormalizzato;     // file riportato su array con eliminazione dei doppi spazi e separazione di tutti i caratteri di punteggiatura
int n_CharFileNormalizzato; // numero caratteri del file normalizzato

char stringone[10000]; // array in cui viene
Record *arrayRecordParole;

void popolaArrayParoleDistinte(char *fin);
void popolaArrayRecordOccorrenze(char *fin);
char *preparaStream(FILE *fin);
int contaCaratteri(FILE *f);
double calcolaOccorrenze(int n, int nTot);
type_parola *getParola(type_parola *arrayPaole, int index);

char *getNomeFile(int args, char *argv[]); // funzione che restituisce il nome file da argomento di avvio
void leggiFile(FILE *fin);

char nomeFile[] = "tempo.txt"; // file di testo di default
FILE *fin;