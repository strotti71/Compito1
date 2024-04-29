#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stringHandler.h"

// int isSeparator(char c);
// char *appendCharToString(char *s, char c);

void stampaStringaformattata(char *s)
{
}

/*
Metodo per pulire una stringa
*/
void pulisciStringa(char *s, int lunghezza)
{
    for (int i = 0; i < lunghezza; i++)
    {
        s[i] = 0;
    }
}
/*
metodo per controllare se il carattere letto è un fine parola...
*/
int isSeparator(char c)
{
    return c == ' ' || c == '\n';
}

/*
metodo che indivisua se il carattere è un carattere di punteggiatura*/
int isPunteggiatura(char c)
{
    return c == ',' || c == '.' || c == '!'|| c == '?';
}

/*
metodo per aggiungere un carattere a una stringa,
    1.creo la stringa temporanea
    2.alloco le dimensioni della stringa originale+ le dim di un carattere
    3. copio nella stringa temporanea la concatenazione
    restituisco la stringa temporanea
char *appendCharToString(char *s, char c)
{
    char *sTemp =NULL;
    size_t newSize = sizeof(s) + sizeof(c);
    // size_t newSize= snprintf(NULL, 0, "%s%s", s, c);
    sTemp = malloc(newSize);
    sprintf(sTemp, "%s%c", s, c);
    return (sTemp);
}*/

char* appendCharToString(char *s, char c, int i)
{
    s[i] = c;
}
/*
il metodo resetta le parole precedente e successiva
*/
void resettaParole(char *parolaPrecedente, char *parolaSuccessiva, int indexChar)
{

    pulisciStringa(parolaPrecedente, _MAX_LENGTH_WORD_);
    strcpy(parolaPrecedente, parolaSuccessiva);
    pulisciStringa(parolaSuccessiva, _MAX_LENGTH_WORD_);
}