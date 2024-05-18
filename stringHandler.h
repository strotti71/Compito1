int isSeparator(char c);
int isPunteggiatura(char c);
void appendCharToString(char *s, char c, int i);
void resettaParole(char *parolaPrecedente, char *parolaSuccessiva, int indexChar);
void pulisciStringa(char *s, int lunghezza);

#define _MAX_LENGTH_WORD_ 31 // lunghezza massima della parola (30 char + \0)
#define MAX_RIGHE 1000000    // Numero massimo di righe del file csv
#define MAX_LEN_RIGA 10000   // numero massimo di caratteri nel csv file

typedef char type_parola[_MAX_LENGTH_WORD_];