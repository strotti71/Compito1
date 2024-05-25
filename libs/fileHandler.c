#include <string.h>
#include "fileHandler.h"

//***********************************************************************
//                                                                      *
//                    LIBRERIA PER LA GESTIONE DEI FILE                 *
//                                                                      *
//***********************************************************************

/// @brief funzione per l'apertura di un file
/// @param filename il nome del file da aprire
/// @param modalita modalità di apertura del file: r o w
/// @return restituisce l'oggetto FILE
FILE *apriFile(char *filename, char *modalita)
{
    printf("\nApro il file %s\n", filename);
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        perror("Errore nell'apertura del file");
    }
    return f;
}

/// @brief funzione per cantare i caratteri in un file di testo.
/// @param f il file di cui si vogliono contare i caratteri
/// @return  il numero di caratteru
int contaCaratteri(FILE *f)
{
    // conto i caratteri del file di input per avere una dimensione minima
    // di caratteri da cui iniziare. i caratteri aggiuntivi (dovuti all'inserimento degli spazi)
    // verranno allocati dopo la scansione di ogni parola
    wint_t c;
    int n = 0;
    while ((c = fgetwc(f)) != WEOF)
    {
        n++;
    }
    rewind(f);
    return n;
}

/// @brief funzione che restituisce una parola dato l'indice dell'array distinctParole di una parola
/// @param number   la posizione nell'array distinct Parole
/// @return         la parola corrispondente
char *getParolaFromnumber(int number)
{
    return "ciao";
}

/// @brief funzione per la creazione del file CSV. La funzione rende in input
///         il record delle parole, quello delle parole distinte e scrive nel file di output
//          per ciascuna parola quella successiva con la percentuale di occorrenze calcolata
/// @param paroleDistinte l'array degli indici delle parole:
/// @param arrayRecParole le parole lette nella loro esatta sequenza
/// @param len             la lunghezza dello stream letto
/// @param fileName        il nome del file di output: da verificare
/// @return                 0 se la funzione non ha generato errori.
int esportaCsv(type_parola_w *paroleDistinte, Record *arrayRecParole, int len, char *fileName)
{
    setlocale(LC_ALL, "");
    int index = 0;
    FILE *file = NULL;
    file = fopen(CSV_FILE, "w");
    if (file == NULL)
    {
        perror("fopen()");
        return EXIT_FAILURE;
    }
    while (index < len)
    {
        if (index > 0)
        {
            fwprintf(file, L"\n%ls", paroleDistinte[index]);
            fflush(stdout);
        }
        else
            fwprintf(file, L"%ls", paroleDistinte[index]);
        for (int j = 0; j < arrayRecParole[index].n_ParoleSuccessive; j++)
        {
            double d = calcolaOccorrenze(arrayRecParole[index].occorrenze[j].n_Occorrenze, arrayRecParole[index].totaleParoleSuccessive);
            fwprintf(file, L",%ls", getParola(paroleDistinte, arrayRecParole[index].occorrenze[j].parolaSuccessiva));

            setlocale(LC_ALL, "C");
            fwprintf(file, L",%5.8f", d);
            //    setlocale(LC_ALL, "");
            fflush(stdout);
        }
        index++;
    }
    fwprintf(file, L"\n");
    fclose(file);
    return 0;
}

/// @brief funzione che restituisce il numero di righe di un file in input
/// @param f    il file di cui si vogliono contare le righe
/// @return     il numero di righe del file
int contarighe(FILE *f)
{
    char c;
    int numRighe = 0;
    while ((c = fgetc(f)) != EOF)
    {
        if (c == '\n')
            numRighe++;
    }
    numRighe++;
    rewind(f);
    return numRighe;
}

/// @brief funzione che, dato un file di testo in input lo stampa a video
/// @param file_path il file che si vuole stampare
void stampa_file(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("Errore nell'apertura del file");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    if (ferror(file))
    {
        perror("Errore nella lettura del file");
    }

    fclose(file);
}
