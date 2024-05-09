#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "fileHandler.h"
#include "stringHandler.h"
#include "occorrenza.h"
#include "printer.h"
/*
#include "fileHandler.c"
#include "stringHandler.c"
#include "occorrenza.c"
#include "printer.c"


1) Apro il file di input
2) copio il contenuto del file in  array(char) fileNormalizzato che ha le seguenti proprietà
    - sono stati rimossi tutti i doppi spazi
    - la punteggiatura viene separata dalle parole precedenti e seguenti tramite uno spazio
3) chiudo il file di input
4) creo arrayParole che contiene le singole istanze di tutte le parole

*/

//***********************************************************************
//                                                                      *
//                    NOMENCLATURA PARTICOLARE PER VARIABILI            *
//                                                                      *
//   n_nomeVariabile:     intero che definisce un numero di occorrenze  *
//   i_nomeVariabile:     intero che definisce un indice                *
//                                                                      *
//***********************************************************************

/********************************
 *                              *
 *          VARIABILI           *
 *                              *
 ********************************/

int nCharInput; // numero caratteri nel file di input

type_parola *arrayParoleDistinte; // Array contiene tutte le singole istanze delle parole
int n_DistinctParoleTesto = 0;    // numero di parole distinte nel testo. ciascuna parola viene caricata nell'array parole
int n_ParoleTotali = 0;
FILE *fin;
char *fileNormalizzato;      // file riportato su array con eliminazione dei doppi spazi e separazione di tutti i caratteri di punteggiatura
int lenFileNormalizzato = 0; // lunghezza del file normalizzato
int n_CharFileNormalizzato;  // numero caratteri del file normalizzato

Record arrayRecordParole[100000];

ProbabilityArray *arrayProb; // Array in cui vengono caricate le probabilità per ciascuna parola

char nomeFile[] = "tempo.txt"; // file di testo di default

//*******************************
//                              *
//          FUNZIONI            *
//                              *
//*******************************

void init();
void leggiParametri(int args, char *argv[]);
void preparaStream(FILE *fin, char *out);
void inserisciCarattere(char *arr, char c, int *n_CharFileNormalizzato);
void popolaArrayParoleDistinte(char *fin);
void popolaArrayRecordOccorrenze(char *fin);
void creaArrayProb(int indexParola);

double calcolaOccorrenze(int n, int nTot);
char *getParola(type_parola *arrayPaole, int index);

char *getNomeFile(int args, char *argv[]); // funzione che restituisce il nome file da argomento di avvio

int main(int args, char *argv[])
{

    init();
    leggiParametri(args, argv);

    printf("%s", nomeFile);

    fin = fopen(nomeFile, "r");
    if (fin == NULL)
    {
        printf("\n\nImpossibile aprire il file %s.\nEsco\n", nomeFile);
        return 1;
    }

    // inizio la lettura del file:
    // la funzione crea un *char filenormalizzato in cui
    // tutte le parole e i segni di interpunzione sono separati da uno spazio
    preparaStream(fin, fileNormalizzato);

    stampaArrayCaratteri(fileNormalizzato, lenFileNormalizzato, "file normalizzato: ");

    // chiudo il file
    if (fin != NULL)
        fclose(fin);

    // la funzione prende il file normalizzato e scrive arrayParole:
    // un array di parole distinte presenti nel testo
    popolaArrayParoleDistinte(fileNormalizzato);

    stampaArrayParole(arrayParoleDistinte, n_DistinctParoleTesto, "\nParole distinte del testo: ");
    printf("\nNumero Parole distinte: %d\nNumero Parole Totali: %d\n", n_DistinctParoleTesto, n_ParoleTotali);

    // creo arrayRecordParole dimensionato con il numero di parole distinte presenti nel testo
    // arrayRecordParole = calloc(n_DistinctParoleTesto, sizeof(Record));
    // arrayRecordParole->n_ParoleSuccessive = 0;

    // costruisco l'array di record che contengono i conteggi delle parole successive
    popolaArrayRecordOccorrenze(fileNormalizzato);

    int index = 0;
    while (index < n_DistinctParoleTesto)
    {
        printf("\n%s", arrayParoleDistinte[index]);
        for (int j = 0; j < arrayRecordParole[index].n_ParoleSuccessive; j++)
        {
            double d = calcolaOccorrenze(arrayRecordParole[index].occorrenze[j].n_Occorrenze, arrayRecordParole[index].totaleParoleSuccessive);
            printf(",%s,%5.3f.", getParola(arrayParoleDistinte, arrayRecordParole[index].occorrenze[j].parolaSuccessiva), d);
            fflush(stdout);
        }
        index++;
    }

    // esportaCsv(arrayRecordParole, numeroParoleTotali, "export.csv");
    index = 0;
    FILE *file = NULL;
    file = fopen("export.csv", "w");
    if (file == NULL)
    {
        perror("fopen()");
        return EXIT_FAILURE;
    }

    while (index < n_DistinctParoleTesto)
    {
        fprintf(file, "\n%s", arrayParoleDistinte[index]);
        for (int j = 0; j < arrayRecordParole[index].n_ParoleSuccessive; j++)
        {
            double d = calcolaOccorrenze(arrayRecordParole[index].occorrenze[j].n_Occorrenze, arrayRecordParole[index].totaleParoleSuccessive);
            fprintf(file, ",%s,%5.3f.", getParola(arrayParoleDistinte, arrayRecordParole[index].occorrenze[j].parolaSuccessiva), d);
            fflush(stdout);
        }
        index++;
    }
    fclose(file);

    // liberare memoria

    creaArrayProb(4);
}

void creaArrayProb(int indexParola)
{ /*
     char *arrayRecord;
     printf("\n\ncarico conteggi dell'indice: %d", indexParola);
     FILE *fileCSV = fopen("export.csv", "r");
     if (fileCSV == NULL)
     {
         perror("creaArrayProb");
         EXIT_FAILURE;
     }
     char c;
     char *riga[31];
     int i = 0;
     int j = 0;
     double prob = 0;
     printf("\n\n");
     char string[2000];
     //  while ((c = fgetc(fin)) != EOF)
     while ((c = fgetc(fileCSV)) != EOF)
     {
         printf("%c", c);
         fflush(stdout);
     }
     int len = arrayRecordParole[indexParola].n_ParoleSuccessive;
     printf("\n\nlunghezza: %d", len);

     fclose(fileCSV);

     arrayProb = (ProbabilityArray *)calloc(100, sizeof(ProbabilityArray));
     for (int i = 0; i < 100; i++)
     {
         printf(" (%d) ", arrayRecordParole[indexParola].occorrenze[i].n_Occorrenze);
     }

     printf("\n\n");
     */
}

void init()
{
    // TODO
    /****************************
     * Funzione init:           *
     * inserire descrizione...  *
     *****************************/
    // printf("\033[H\033[J");
}

void leggiParametri(int args, char *argv[])
{
    // TODO
    //*******************************
    // Funzione leggiParametri:     *
    // La funzione analizza         *
    // i parametri del main         *
    //*******************************
    strcpy(nomeFile, getNomeFile(args, argv));
}

double calcolaOccorrenze(int n, int nTot)
{
    return ((double)n / nTot);
    // return (n);
}

char *getNomeFile(int args, char *argv[])
{
    /*
La funzione restituisce il nome file
    se è specificato il primo argomento in argv[] allora  l'argomento è il nome del file
    altrimenti viene restituito un file di default
*/
    return (args > 1 ? argv[1] : nomeFile);
}

void preparaStream(FILE *fin, char *out)
{
    //********************************************************************************************************************
    //*****il metodo legge il file e copia tutte le parole nell'array fileNormalizzato eseguendo le seguenti correzioni:
    //*****1- conto i caratteri del file in input (serviranno per dimensionare fileNormalizzato
    //*****1- inserisce spazi prima e dopo la punteggiatura
    //*****2- elimina doppi spazi
    //***** al termine dell'elaborazione ciascuna parola o segno di punteggiatura sono separati da uno spazio
    //********************************************************************************************************************
    char c;
    char cPrec = ' ';

    n_CharFileNormalizzato = 0;
    lenFileNormalizzato = contaCaratteri(fin);
    rewind(fin);

    out = malloc(sizeof(char) * lenFileNormalizzato);

    inserisciCarattere(out, '.', &n_CharFileNormalizzato);
    inserisciCarattere(out, ' ', &n_CharFileNormalizzato);

    while ((c = fgetc(fin)) != EOF)
    {
        if (isPunteggiatura(c))
        {
            if (cPrec != ' ')
            {
                inserisciCarattere(out, 32, &n_CharFileNormalizzato);
                cPrec = ' ';
            }
            inserisciCarattere(out, c, &n_CharFileNormalizzato);
            inserisciCarattere(out, 32, &n_CharFileNormalizzato);
            cPrec = ' ';
        }

        // caratteri < 40 sono spazi
        else if (c < 32)
        {
            if (cPrec != ' ')
            {
                inserisciCarattere(out, 32, &n_CharFileNormalizzato);
                cPrec = ' ';
            }
        }
        else
        {
            // se il carattere precedente e il carattere letto sono entrambi spazi,
            // non inserisco il carattere per evetiare doppi spazi inutili
            c = tolower(c);
            if (!((cPrec == c) && (c == 32)))
            {
                inserisciCarattere(out, c, &n_CharFileNormalizzato);
            }
            cPrec = c;
        }
    }
    fileNormalizzato = out;
}

void inserisciCarattere(char *arr, char c, int *n_CharFileNormalizzato)
{
    // se la lunghezza di fileNormalizzato è minore dell'indice di lettura
    // espando l'array di un carattere
    if (lenFileNormalizzato == *n_CharFileNormalizzato)
    {
        lenFileNormalizzato += 1;
        arr = (char *)realloc(arr, sizeof(char) * lenFileNormalizzato);
    }

    appendCharToString(arr, c, *n_CharFileNormalizzato);
    *n_CharFileNormalizzato += 1;
}

void popolaArrayRecordOccorrenze(char *testo)
{
    /* seconda lettura del file: popolo l'array di record
per ciascuna parola che leggo nel file:
    - cerco l'indice sull'arrayParole
       (es: quel = 1)
       cerco l'indice in record[].parola
           - se non è presente inserisco l'indice in coda all'array
    da terminare per le parole successive e le occorrenze
*/
    char c;
    int i_Char = 0;
    int i_Testo = 0;

    int i_Precedente = -1;
    int i_Successiva;

    int indiceParolaInArrayStructParole = 0;
    type_parola parolaPrecedente = "."; // l'ultima parola letta
    type_parola parolaSuccessiva;       // la parola che sto leggendo

    pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);

    while (i_Testo < n_CharFileNormalizzato)
    {

        c = testo[i_Testo];
        if (isSeparator(c))
        {
            if (i_Testo > 1) // se sono nel primo carattere '.' non ho parole precedenti
            {
                // chiudo la parola che sto costruendo
                appendCharToString(parolaSuccessiva, '\0', i_Char);

                // cerco l'indice della parola appena letta nella'rray parole
                i_Successiva = cercaParola(arrayParoleDistinte, parolaSuccessiva, n_DistinctParoleTesto);

                // cerco l'indice della parola precedente
                i_Precedente = cercaParola(arrayParoleDistinte, parolaPrecedente, n_DistinctParoleTesto);
                // creo un nuovo record copia del record corrispondente alla parola precedente

                Record RecordParolaTemp = arrayRecordParole[i_Precedente];

                int trovato = 0;
                for (int i = 0; (i < RecordParolaTemp.n_ParoleSuccessive && !trovato); i++)
                {
                    if (RecordParolaTemp.occorrenze[i].parolaSuccessiva == i_Successiva)
                    {
                        RecordParolaTemp.occorrenze[i].n_Occorrenze = RecordParolaTemp.occorrenze[i].n_Occorrenze + 1; //= arrayRecordParole->occorrenze[i].numeroOccorrenze + 1;
                        RecordParolaTemp.totaleParoleSuccessive++;                                                     //= arrayRecordParole->n_ParoleSuccessive + 1;
                        trovato = 1;
                        arrayRecordParole[i_Precedente] = RecordParolaTemp;
                    }
                }
                // non ho trovato occorrenza successiva:
                if (!trovato)
                {
                    // 1 creo array più grande
                    int newCountOccorrenze = RecordParolaTemp.n_ParoleSuccessive + 1;
                    if (newCountOccorrenze == 1)
                    {
                        RecordParolaTemp.occorrenze = malloc(sizeof(Occorrenza) * 1);
                        if (!RecordParolaTemp.occorrenze)
                        {
                            perror("realloc fallita...");
                            exit(EXIT_FAILURE);
                        }
                    }

                    else
                        // rialloco la memoria per arrayTemp.occorrenze[];
                        RecordParolaTemp.occorrenze = (Occorrenza *)realloc(RecordParolaTemp.occorrenze, newCountOccorrenze * sizeof(Occorrenza));

                    if (!RecordParolaTemp.occorrenze)
                    {
                        perror("realloc fallita...");
                        exit(EXIT_FAILURE);
                    }

                    // printf("\n%s %s %d\n", parolaPrecedente, parolaSuccessiva, RecordParolaTemp.occorrenze[newCountOccorrenze].numeroOccorrenze);
                    // fflush(stdout);

                    RecordParolaTemp.occorrenze[newCountOccorrenze - 1].parolaSuccessiva = i_Successiva;
                    RecordParolaTemp.occorrenze[newCountOccorrenze - 1].n_Occorrenze = 1;
                    RecordParolaTemp.n_ParoleSuccessive++;
                    RecordParolaTemp.totaleParoleSuccessive++;

                    /*                 Occorrenza *newOcc = (Occorrenza *)calloc(newCountOccorrenze, sizeof(Occorrenza));
                                 for (int i = 0; i < RecordParolaTemp.n_ParoleSuccessive; i++)

                                 {
                                     newOcc[i] = RecordParolaTemp.occorrenze[i];
                                 }

                 newOcc[newCountOccorrenze].parolaSuccessiva = indiceSuccessiva;
                 newOcc[newCountOccorrenze].numeroOccorrenze = 1;

                 free(RecordParolaTemp.occorrenze);
    */

                    // RecordParolaTemp.occorrenze = newOcc;
                    //   RecordParolaTemp.n_ParoleSuccessive++;
                    // arrayRecordParole[indicePrecedente] = RecordParolaTemp;
                    //  printf("\nPrecedente:'%s'\tSuccessiva:'%s':\t num parole successive:%d", parolaPrecedente, parolaSuccessiva, arrayRecordParole->n_ParoleSuccessive);

                    indiceParolaInArrayStructParole++;

                    arrayRecordParole[i_Precedente] = RecordParolaTemp;

                    // arrayRecordParole[indicePrecedente].occorrenze = realloc(arrayRecordParole[indicePrecedente].occorrenze, newCountOccorrenze * sizeof(Occorrenza));
                }
                // rialloco lo spazio per il record indice precedente

                /*  printf("\n%d\n", arrayRecordParole[indicePrecedente].occorrenze[1].numeroOccorrenze);
                  fflush(stdout);
      */
                pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
                strcpy(parolaPrecedente, parolaSuccessiva);
                pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);

                i_Testo++;
                i_Char = 0;
            }
            else
            {
                i_Testo++;
                i_Char = 0;
            }
        }
        else
        {
            if (i_Char < _MAX_LENGTH_WORD_)
            {
                parolaSuccessiva[i_Char] = c;
                i_Char++;
                i_Testo++;
            }
            else
                printf("BREAK");
        }
    }
}

void popolaArrayParoleDistinte(char *fin)
{
    /*
Metodo per la prima lettura del file.
il metodo legge il file fino alla fine e scrive tutte le parole trovate in un distinctArray
*/

    char c;
    int i_Char = 0;
    int i_Fin = 0;
    type_parola parola; //= ".";
    arrayParoleDistinte = (type_parola *)calloc(1, sizeof(type_parola));
    if (arrayParoleDistinte == NULL)
        printf("Abort");

    // inserisciParola(arrayParoleDistinte, parola, n_ParoleTotali);
    // n_ParoleTotali++;

    for (int i = 0; i < lenFileNormalizzato; i++)
    {
        c = (fin[i_Fin]);
        if (!((c < 32) || (c > 126)))
        {

            if (isSeparator(c))
            {
                // se ho trovato un separatore ho una nuova parola e la inserisco nell'array

                if (i_Char > 0)
                {
                    appendCharToString(parola, '\0', i_Char);
                    n_ParoleTotali++;

                    if (cercaParola(arrayParoleDistinte, parola, n_DistinctParoleTesto) < 0)
                    {
                        n_DistinctParoleTesto++;
                        arrayParoleDistinte = (type_parola *)realloc(arrayParoleDistinte, sizeof(type_parola) * n_DistinctParoleTesto);
                        inserisciParola(arrayParoleDistinte, parola, n_DistinctParoleTesto - 1);
                    }
                    pulisciStringa(parola, _MAX_LENGTH_WORD_);
                    i_Char = 0;
                }
            }
            else if (isPunteggiatura(c))
            {
                appendCharToString(parola, c, i_Char);

                //  parola[i_Char] = c;
                i_Char++;
                appendCharToString(parola, '\0', i_Char);
                n_ParoleTotali++;

                if (cercaParola(arrayParoleDistinte, parola, n_DistinctParoleTesto) < 0)
                {
                    n_DistinctParoleTesto++;
                    arrayParoleDistinte = (type_parola *)realloc(arrayParoleDistinte, sizeof(type_parola) * n_DistinctParoleTesto);

                    inserisciParola(arrayParoleDistinte, parola, n_DistinctParoleTesto - 1);
                }
                pulisciStringa(parola, _MAX_LENGTH_WORD_);
                i_Char = 0;
            }
            else
            {
                if (i_Char < _MAX_LENGTH_WORD_)
                {
                    parola[i_Char] = c;
                    i_Char++;
                }
            }
        }
        i_Fin++;
    }
}

char *getParola(type_parola *arrayParole, int index)
{
    return (arrayParole[index]);
}
