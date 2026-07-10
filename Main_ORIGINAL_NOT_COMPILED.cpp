/*
 * Main dell'applicazione.
 */
  
#include <stdio.h>
#include <stdlib.h>
#include "CommonData.h"
#include "ReadInputGraph.h"
#include "Population.h"
#include "TabuSearch.h"
#include "Utility.h"



/***********************************************************************
 *                                                                     *
 * Main dell'applicazione che avvia la procedura di risoluzione.       *
 *                                                                     *
 ***********************************************************************/
void main(int argc, char** argv){

	
	/* controllo parametri della riga di comando */
	if(argc != 10){
		printf("Uso:\n%s instanceName K tabuSearchIterations parameterAlpha parameterRand parameterConst populationSize timeLimit random_seed\n", argv[0]);
		exit(1);
	}
	
	/* nome dell'istanza */
	instanceName=argv[1];

	/* target K */
	KColorNumber=atoi(argv[2]);
	if(KColorNumber < 1){
		puts("Errore nel target K.");
		exit(2);
	}

	/* iterazioni del tabu search */
	tabuSearchIterations=atoi(argv[3]);
	if(tabuSearchIterations < 1){
		puts("Errore nel numero di iterazioni del Tabu Search.");
		exit(3);
	}

	/* parametro alpha per il calcolo del tabu tenure */
	parameterAlpha=atof(argv[4]);
	if(parameterAlpha < 0){
		puts("Errore in parameterAlpha.");
		exit(4);
	}

	/* parametro A per il calcolo del tabu tenure */
	parameterRand=atoi(argv[5]);
	if(parameterRand < 0){
		puts("Errore in parameterRand.");
		exit(5);
	}

	/* parametro const per il calcolo del tabu tenure */
	parameterConst=atoi(argv[6]);
	if(parameterConst < 0){
		puts("Errore in parameterConst.");
		exit(6);
	}

	/* dimensione della popolazione */
	populationSize=atoi(argv[7]);
	if(populationSize < 2){
		puts("Errore nella dimensione della popolazione.");
		exit(7);
	}
	
	/* time limit */
	timeLimit=atoi(argv[8]);
	if(timeLimit < 1){
		puts("Errore nel time limit.");
		exit(9);
	}

		/* random function seed */
	set_seed=atoi(argv[9]);
	if(set_seed < 0){
		puts("Errore nel seme random");
		exit(10);
	}
	

	/* lettura dell'istanza */
	readInputGraph(argv[1]);

	printf("instanceName = %s\nKColorNumber = %d\ntabuSearchIterations = %d\nparameterAlpha = %f\nparameterRand = %d\nparameterConst = %d\npopulationSize = %d\ntimeLimit = %d\nrandom_seed = %d\n\n",instanceName,KColorNumber,tabuSearchIterations,parameterAlpha,parameterRand,parameterConst,populationSize,timeLimit,set_seed);

	/* inizio conteggio del tempo */
	startTiming();

	/* inizializzazione del generatore di numeri random */
	initializeRandomGenerator();

	/* inizializzazione della popolazione */
	initPopulation();

	/* ciclo di generazione */
	generationCycle();

	/* se il ciclo di generazione non e' stato interrotto da una chiamata
	   alla procedura "feasibleSolutionFound()", la soluzione non e'
	   stata trovata */
	puts("\nNessuna soluzione ammissibile trovata.");

	/* fine conteggio del tempo */
	stopTiming();

	/* stampa del tempo trascorso */
	printElapsedTime();
	
	/* deallocazione memoria della popolazione */
	freePopulation();

}//main
