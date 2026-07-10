/*
 * Dichiarazione di funzioni di utilita' generale.
 */


#ifndef UTILITY_HEADER
#define UTILITY_HEADER

#include <string>
#include <vector>

/* definizione del tipo boolean */

//#ifndef BOOLEAN
//#define BOOLEAN
//typedef enum {FALSE=0, TRUE} boolean;
//#endif


/* seme per il generatore di numeri random */
#define RANDOM_SEED 1000



/***********************************************************************
 *                                                                     *
 * Inizio conteggio del tempo.                                         *
 *                                                                     *
 ***********************************************************************/
void startTiming();


/***********************************************************************
 *                                                                     *
 * Fine conteggio del tempo.                                           *
 *                                                                     *
 ***********************************************************************/
void stopTiming();


/***********************************************************************
 *                                                                     *
 * Stampa del tempo trascorso.                                         *
 *                                                                     *
 ***********************************************************************/
void printElapsedTime();

/***********************************************************************
 *                                                                     *
 * Stampa del tempo trascorso.        enrico                                 *
 *                                                                     *
 ***********************************************************************/
double printElapsedTime2();

/***********************************************************************
 *                                                                     *
 * Stampa del tempo trascorso.        pss                              *
 *                                                                     *
 ***********************************************************************/
double getElapsedTime();


/***********************************************************************
 *                                                                     *
 * Determina se e' stato raggiunto il time limit.                      *
 *                                                                     *
 ***********************************************************************/
bool isTimeLimit();


/***********************************************************************
 *                                                                     *
 * Inizializzazione del generatore di numeri random.                   *
 *                                                                     *
 ***********************************************************************/
void initializeRandomGenerator();


/***********************************************************************
 *                                                                     *
 * Restituisce un numero random nell'intervallo [0, "range"[           *
 * (estremo finale escluso).                                           *
 *                                                                     *
 ***********************************************************************/

int getRandomNumber(int range);





/* Enrico 12.10.04  ****************************************
*                                                          *
*  stampa su file la soluzione fesible                     *
*                                                          *
************************************************************/

void print_file(std::string instance_name, int *solution);


/*PSS 03.11.21****************************************************
*																	*
*   sol2vector														*
*                                                                   *
*********************************************************************/
void sol2vector(int* col, size_t size, std::vector<int>& vcol);

#endif