/*
 * Main dell'applicazione.
 */
  
//#include <stdio.h>
//#include <stdlib.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "CommonData.h"
#include "ReadInputGraph.h"
#include "Population.h"
#include "TabuSearch.h"
#include "Utility.h"


#ifndef __RUN_VCP_MANAGER_H__
#define __RUN_VCP_MANAGER_H__

class heurVCP {
public:
	static int mmt_VCPheur(std::string name, int target_K, info_VCPh& info, int ITER_TABU = 10000, double alfa_TABU = 1, int rand_TABU = 1, 
							int const_TABU =1 , int POP_SIZE = 10, int TIME_LIMIT = 10, int random_SEED = RANDOM_SEED, string filename = LOG_VCPH_FILENAME,  bool verbose = true		);

	static int mmt_VCPheur(ugraph& ug, int target_K, info_VCPh& info, int ITER_TABU = 10000, double alfa_TABU = 1, int rand_TABU = 1,
							int const_TABU = 1, int POP_SIZE = 10, int TIME_LIMIT = 10, int random_SEED = RANDOM_SEED, string filename= LOG_VCPH_FILENAME, bool verbose = true);
};

int heurVCP::mmt_VCPheur(std::string name, int target_K, info_VCPh& info, int ITER_TABU, double alfa_TABU, int rand_TABU, 
							int const_TABU, int POP_SIZE, int TIME_LIMIT, int random_SEED, string filename, bool verbose		) {

	info.reset();
	info.name = name;
	instanceName = (char*) name.c_str();


	/* target K */
	KColorNumber = target_K;
	if (KColorNumber < 1) {
		puts("Errore nel target K.");
		exit(2);
	}
	info.target_K = target_K;

	/* iterazioni del tabu search */
	tabuSearchIterations = ITER_TABU;
	if (tabuSearchIterations < 1) {
		puts("Errore nel numero di iterazioni del Tabu Search.");
		exit(3);
	}
	info.ITER_TABU = ITER_TABU;

	/* parametro alpha per il calcolo del tabu tenure */
	parameterAlpha = alfa_TABU;
	if (parameterAlpha < 0) {
		puts("Errore in parameterAlpha.");
		exit(4);
	}
	info.ALFA_TABU = alfa_TABU;

	/* parametro A per il calcolo del tabu tenure */
	parameterRand = rand_TABU;
	if (parameterRand < 0) {
		puts("Errore in parameterRand.");
		exit(5);
	}
	info.RAND_TABU = rand_TABU;

	/* parametro const per il calcolo del tabu tenure */
	parameterConst = const_TABU;
	if (parameterConst < 0) {
		puts("Errore in parameterConst.");
		exit(6);
	}
	info.CONST_TABU = const_TABU;

	/* dimensione della popolazione */
	populationSize = POP_SIZE;
	if (populationSize < 2) {
		puts("Errore nella dimensione della popolazione.");
		exit(7);
	}
	info.pop_size= POP_SIZE;

	/* time limit */
	timeLimit = TIME_LIMIT;
	if (timeLimit < 1) {
		puts("Errore nel time limit.");
		exit(9);
	}
	info.TIME_LIMIT = TIME_LIMIT;

	/* random function seed */
	set_seed = random_SEED;
	if (set_seed < 0) {
		puts("Errore nel seme random");
		exit(10);
	}
	info.random_seed = random_SEED;
	

	/* lettura dell'istanza */
	readInputGraph(instanceName);

	//I/O
	if (verbose) {
		all_info_VCPh.print_params(std::cout);
	}

#ifdef print_original_logs_vcph
	printf("instanceName = %s\nKColorNumber = %d\ntabuSearchIterations = %d\nparameterAlpha = %f\nparameterRand = %d\nparameterConst = %d\npopulationSize = %d\ntimeLimit = %d\nrandom_seed = %d\n\n", 
			instanceName, KColorNumber, tabuSearchIterations, parameterAlpha, parameterRand, parameterConst, populationSize, timeLimit, set_seed															);
#endif

	/* inizio conteggio del tempo */
	startTiming();

	/* inizializzazione del generatore di numeri random */
	initializeRandomGenerator();
	

	/* inizializzazione della popolazione */
	initPopulation();
		
	/* ciclo di generazione */
	if (!all_info_VCPh.is_sol_found) {
		generationCycle();
	}
	

	/* se il ciclo di generazione non e' stato interrotto da una chiamata
	   alla procedura "feasibleSolutionFound()", la soluzione non e'
	   stata trovata */
	if(!all_info_VCPh.is_sol_found)
		puts("\n Nessuna soluzione ammissibile trovata.");

	/* fine conteggio del tempo */
	stopTiming();

	/* stampa del tempo trascorso */
#ifdef print_logs_vcph
	printElapsedTime2();
#endif

	all_info_VCPh.elapsed_time = getElapsedTime();

	///////////////
	//I/O
	if (verbose) {
		all_info_VCPh.print_sol(std::cout);	
	}
	
	if (!filename.empty()) {
		ofstream f(filename, ofstream::app);
		if (!f) { LOG_ERROR("log error"); }
		f << info;
		f.close();
	}
	
	/* PSS-TABU search dealloc */
	freeMemoryTabu();						

	/* deallocazione memoria della popolazione */
	freePopulation();

	/* PSS-free graph */
	free(pointerVector);			pointerVector = NULL;
	free(adjacentVector);			adjacentVector = NULL;
	free(adjacentNumberVector);		adjacentNumberVector = NULL;

	return 0;
}

int heurVCP::mmt_VCPheur(ugraph& ug, int target_K, info_VCPh& info, int ITER_TABU, double alfa_TABU, int rand_TABU, int const_TABU, int POP_SIZE, int TIME_LIMIT, int random_SEED, string filename, bool verbose) {

	info.reset();
	info.name = ug.get_name();
	instanceName = (char*) ug.get_name().c_str();
	

	/* target K */
	KColorNumber = target_K;
	if (KColorNumber < 1) {
		puts("Errore nel target K.");
		exit(2);
	}
	info.target_K = target_K;

	/* iterazioni del tabu search */
	tabuSearchIterations = ITER_TABU;
	if (tabuSearchIterations < 1) {
		puts("Errore nel numero di iterazioni del Tabu Search.");
		exit(3);
	}
	info.ITER_TABU = ITER_TABU;

	/* parametro alpha per il calcolo del tabu tenure */
	parameterAlpha = alfa_TABU;
	if (parameterAlpha < 0) {
		puts("Errore in parameterAlpha.");
		exit(4);
	}
	info.ALFA_TABU = alfa_TABU;

	/* parametro A per il calcolo del tabu tenure */
	parameterRand = rand_TABU;
	if (parameterRand < 0) {
		puts("Errore in parameterRand.");
		exit(5);
	}
	info.RAND_TABU = rand_TABU;

	/* parametro const per il calcolo del tabu tenure */
	parameterConst = const_TABU;
	if (parameterConst < 0) {
		puts("Errore in parameterConst.");
		exit(6);
	}
	info.CONST_TABU = const_TABU;

	/* dimensione della popolazione */
	populationSize = POP_SIZE;
	if (populationSize < 2) {
		puts("Errore nella dimensione della popolazione.");
		exit(7);
	}
	info.pop_size = POP_SIZE;

	/* time limit */
	timeLimit = TIME_LIMIT;
	if (timeLimit < 1) {
		puts("Errore nel time limit.");
		exit(9);
	}
	info.TIME_LIMIT = TIME_LIMIT;

	/* random function seed */
	set_seed = random_SEED;
	if (set_seed < 0) {
		puts("Errore nel seme random");
		exit(10);
	}
	info.random_seed = random_SEED;


	/* lettura dell'istanza */
	readInputGraph(ug);

	//I/O
	if (verbose) {
		all_info_VCPh.print_params(std::cout);
	}

#ifdef print_original_logs_vcph
	printf("instanceName = %s\nKColorNumber = %d\ntabuSearchIterations = %d\nparameterAlpha = %f\nparameterRand = %d\nparameterConst = %d\npopulationSize = %d\ntimeLimit = %d\nrandom_seed = %d\n\n",
		instanceName, KColorNumber, tabuSearchIterations, parameterAlpha, parameterRand, parameterConst, populationSize, timeLimit, set_seed);
#endif

	/* inizio conteggio del tempo */
	startTiming();

	/* inizializzazione del generatore di numeri random */
	initializeRandomGenerator();


	/* inizializzazione della popolazione */
	initPopulation();
	   

	/* ciclo di generazione */
	if (!all_info_VCPh.is_sol_found) {
		generationCycle();
	}



	/* se il ciclo di generazione non e' stato interrotto da una chiamata
	   alla procedura "feasibleSolutionFound()", la soluzione non e'
	   stata trovata */
	if (!all_info_VCPh.is_sol_found)
		puts("\n Nessuna soluzione ammissibile trovata.");

	/* fine conteggio del tempo */
	stopTiming();
		

	/* stampa del tempo trascorso */
#ifdef print_logs_vcph
	printElapsedTime2();
#endif

	all_info_VCPh.elapsed_time = getElapsedTime();

	////////////////////
	//I/O
	if (verbose) {
		all_info_VCPh.print_sol(std::cout);	
	}

	if (!filename.empty()) {
		ofstream f(filename, ofstream::app);
		if (!f) { LOG_ERROR("log error"); }
		f << info;
		f.close();
	}

	//////////////////////////////////////
	// deallocation

	/* PSS-Tabu search dealloc  */
	freeMemoryTabu();

	/* deallocazione memoria della popolazione */
	freePopulation();

	/* PSS-free graph */
	free(pointerVector);			pointerVector = NULL;
	free(adjacentVector);			adjacentVector = NULL;
	free(adjacentNumberVector);		adjacentNumberVector = NULL;

	/////////////////////////////////////////
			
	
	return 0;
}

#endif // !__RUN_VCP_MANAGER_H__