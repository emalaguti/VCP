/*
 * Dichiarazione di funzioni per la gestione della popolazione.
 */

#include "Utility.h"

#ifndef POPULATION_HEADER
#define POPULATION_HEADER


/* dimensione della popolazione */
extern int populationSize;


/* limite di tempo per le generazioni */
extern int timeLimit;


/* vettore di configurazioni (coloring) non ammissibili */
extern int** population;


/* vettore contenente i valori delle soluzioni corrispondenti
   ad ogni configurazione (parent) contenuta nella popolazione
   (utilizzato per efficienza) */
extern int* parentValueVector;



/***********************************************************************
 *                                                                     *
 * Inizializzazione di un pool di configurazioni di coloring.          *
 *                                                                     *
 ***********************************************************************/
void initPopulation();


/***********************************************************************
 *                                                                     *
 * Deallocazione della memoria utilizzata dalla popolazione.           *
 *                                                                     *
 ***********************************************************************/
void freePopulation();


/***********************************************************************
 *                                                                     *
 * Scelta random di due genitori contenuti nella popolazione.          *
 * I parametri di ritorno sono "indexParentS1" e "indexParentS2", che  *
 * rappresentano gli indici dei genitori scelti all'interno del        *
 * vettore della popolazione.                                          *
 *                                                                     *
 ***********************************************************************/
void chooseParents(int* indexParentS1, int* indexParentS2);


/***********************************************************************
 *                                                                     *
 * Aggiornamento della popolazione tramite l'inserimento della nuova   *
 * configurazione creata (e migliorata dal Tabu Search), che viene     *
 * sostituita al peggiore dei due parent che l'hanno generata          *
 * (cioe', quello con il maggiore valore della soluzione).             *
 * In ingresso si hanno gli indici dei due parent all'interno della    *
 * popolazione e il coloring dell'offspring.                           *
 *                                                                     *
 ***********************************************************************/
void updatePopulation(int indexParentS1, int indexParentS2, int* offspring);


/***********************************************************************
 *                                                                     *
 * Ritorna il valore della soluzione corrispondente alla               *
 * configurazione di coloring passata in ingresso, calcolando la       *
 * sommatoria del grado (= numero lati incidenti) dei vertici          *
 * non colorati.                                                       *
 *                                                                     *
 ***********************************************************************/
int getSolutionValue(int* coloring);


/***********************************************************************
 *                                                                     *
 * Ciclo di generazione.                                               *
 *                                                                     *
 ***********************************************************************/
void generationCycle();



#endif

