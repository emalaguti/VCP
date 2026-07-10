/*
 * Dichiarazione della funzione di Tabu Search.
 */


#ifndef TABU_SEARCH_HEADER
#define TABU_SEARCH_HEADER


/* numero di iterazioni */
extern int tabuSearchIterations;

/* lunghezza della lista tabu */
extern int tabuTenure;

/* parametri per il calcolo del tabuTenure */
extern double parameterAlpha;
extern int parameterRand;
extern int parameterConst;

/* PSS */
void freeMemoryTabu();			


/***********************************************************************
 *                                                                     *
 * Tabu Search.                                                        *
 * "coloring" e' parametro di ingresso-uscita: la configurazione       *
 * iniziale da migliorare viene aggiornata con la migliore             *
 * colorazione trovata. Ritorna anche il valore corrispondente         *
 * di tale soluzione.                                                  *
 *                                                                     *
 ***********************************************************************/
int tabuSearch(int* coloring);


#endif
