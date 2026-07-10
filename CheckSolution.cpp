/*
 * Controllo sull'ammissibilita' della soluzione. Si verifica che ogni vertice
 * non abbia un adiacente con la sua stessa classe di colore.
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "CommonData.h"
#include "Utility.h"
#include "Population.h"


/***********************************************************************
 *                                                                     *
 * Ritorna true se "bestColoring" e' una soluzione ammissibile.        *
 *                                                                     *
 ***********************************************************************/
bool checkSolution(int* bestColoring){

	/* indici generici */
	int i, j;

	/* colore del vertice in esame */
	int currentColor;

	/* indica se la soluzione e' ammissibile */
	bool feasibleSolution;

	/* indici per esaminare gli adiacenti */
	int adjacent;         //adiacente in esame
	int adjacentFrom;     //indice del primo adiacente in adjacentVector
	int adjacentTo;       //indice dell'ultimo adiacente

	/* numero di vertici non colorati */
	int uncoloredNumber;

	/* per verificare se il numero di colori effettivamente utilizzati e' minore di K */
	int* colorUse;


	feasibleSolution=true;
	uncoloredNumber=0;

	for(i=0; i<vertexNumber; i++){        //per ogni vertice

		currentColor=bestColoring[i];
		
		if(currentColor <= 0){            //controllo che sia colorato
			feasibleSolution=false;
			uncoloredNumber++;
			printf("Vertice %d non colorato.\n", i);
			continue;  //passo al vertice successivo
		}//if

		if(currentColor > KColorNumber){  //classe di colore superiore a K
			feasibleSolution=false;
			printf("Vertice %d utilizza un colore non ammesso (#%d).\n", i, currentColor);
			continue;  //passo al vertice successivo
		}//if
		
		adjacentFrom=pointerVector[i];
		adjacentTo=pointerVector[i+1]-1;

		for(j=adjacentFrom; j<=adjacentTo; j++){       //per ogni adiacente
			adjacent=adjacentVector[j];
			if(currentColor == bestColoring[adjacent]){
				feasibleSolution=false;
				printf("Vertici %d e %d hanno lo stesso colore (#%d).\n",
					i, adjacent, currentColor);
			}//if
		}//for
	}//for

	/* stampo il numero di vertici non colorati */
	if(feasibleSolution == false)
		printf("Numero di vertici non colorati: %d\n", uncoloredNumber);


	/* verifico se in realta' i colori utilizzati sono meno di K */
	if(feasibleSolution){
		colorUse=(int*)calloc(KColorNumber+1, sizeof(int));
		if(colorUse==NULL){
			puts("checkSolution: out of memory");
			exit(1);
		}
		for(i=1; i<=KColorNumber; i++) colorUse[i]=false;  //nessun colore utilizzato

		for(i=0; i<vertexNumber; i++)
			colorUse[bestColoring[i]]=true;    //colore utilizzato

		j=KColorNumber;  //numero di colori utilizzati
		for(i=1; i<=KColorNumber; i++)
			if(colorUse[i]==false) j--;

		if(j<KColorNumber) printf("La soluzione utilizza solamente %d classi di colore.\n", j);

		free(colorUse);
	}//if

	return feasibleSolution;
}



/***********************************************************************
 *                                                                     *
 * Procedura chiamata quando viene trovata una soluzione ammissibile.  *
 * Puo' essere invocata al termine delle fasi di GreedyColoring,       *
 * di Crossover e di TabuSearch.                                       *
 * "phaseName" e' il nome della fase che ha trovato la soluzione.      *
 *                                                                     *
 ***********************************************************************/
int feasibleSolutionFound(int* feasibleSolution, char* phaseName){

	/* fine conteggio del tempo */
	stopTiming();

	/* controllo della soluzione */
	int status=checkSolution(feasibleSolution);
	if (status == false) {
		std::cout << "bizarre solution-feasibleSolutionFound()..." << std::endl;
		return -1;
	}

	printf("\nTrovata soluzione ammissibile durante la fase: %s.\n", phaseName);

//////////////////////////porcheria/////////////////////////
//	FILE* fp; 
//	double tempo;

 /* apertura del file in scrittura */
//   fp = fopen ("risultati_gen.txt","a+");
   //fp = fopen(info_VCPh.log_detailed_results_filename.c_str(), "a+");
   ////tempo=printElapsedTime2();
   //tempo = getElapsedTime();
   //fprintf(fp,"instanceName = %s  KColorNumber = %d  random_seed = %d  time = %f\n", instanceName, KColorNumber,set_seed,tempo);
   //fclose (fp);

	///////////////////////////////////////////////////////////////////

/* Enrico 12.10.04 - pss 03/11/21 */
 //   print_file(instanceName, feasibleSolution);

	/* store solution */
	// sol2vector(feasibleSolution, vertexNumber, info_VCPh.sol);

	/* stampa del tempo trascorso */
	//printElapsedTime();
	
	/* deallocazione memoria della popolazione */
	//freePopulation();
	
	/* deallocazione memoria della soluzione */
	//  free(feasibleSolution);

	/* terminazione corretta dell'applicazione */
	//exit(0);
	return 0;


}//feasibleSolutionFound
