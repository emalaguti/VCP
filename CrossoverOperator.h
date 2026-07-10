/*
 * Dichiarazione della procedura di crossover.
 */


#ifndef CROSSOVER_OPERATOR_HEADER
#define CROSSOVER_OPERATOR_HEADER

/***********************************************************************
 *                                                                     *
 * "parentS1" e "parentS2" sono le configurazioni di colore dei        *
 * genitori.                                                           *
 * Il vettore "offspring" conterra' il figlio creato.                  *
 *                                                                     *
 ***********************************************************************/
void crossover(int* parentS1, int* parentS2, int* offspring);


#endif
