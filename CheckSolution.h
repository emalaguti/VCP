/*
 * Dichiarazione della procedura di controllo della soluzione.
 */

#ifndef CHECK_SOLUTION_HEADER
#define CHECK_SOLUTION_HEADER

#include "CommonData.h"  //per "boolean"


/***********************************************************************
 *                                                                     *
 * Ritorna TRUE se "bestColoring" e' una soluzione ammissibile.        *
 *                                                                     *
 ***********************************************************************/
bool checkSolution(int* bestColoring);


/***********************************************************************
 *                                                                     *
 * Procedura chiamata quando viene trovata una soluzione ammissibile.  *
 * Puo' essere invocata al termine delle fasi di GreedyColoring,       *
 * di Crossover e di TabuSearch.                                       *
 * "phaseName" e' il nome della fase che ha trovato la soluzione.      *
 *                                                                     *
 ***********************************************************************/
int feasibleSolutionFound(int* feasibleSolution, char* phaseName);


#endif
