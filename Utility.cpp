/*
 * Definizione di funzioni di utilita' generale.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include "Utility.h"
#include "Population.h"
#include "CommonData.h"
#include "TabuSearch.h"



/* Enrico 12.10.04 */
#define PATH_OUTPUT "Soluzioni"

#define PATH_SEPARATOR "\\"

//pss 03.11.2021
//#define LOG_SOL_TO_FILE "info_VCPheur_sol.txt"


/* variabili per il timing */
time_t startTime;
time_t stopTime;
time_t tempTime;


/***********************************************************************
 *                                                                     *
 * Inizio conteggio del tempo.                                         *
 *                                                                     *
 ***********************************************************************/
void startTiming(){
	time(&startTime);
#ifdef print_original_logs_vcph
	printf("startTime: %s\n", ctime(&startTime));
#endif
}


/***********************************************************************
 *                                                                     *
 * Fine conteggio del tempo.                                           *
 *                                                                     *
 ***********************************************************************/
void stopTiming(){
	time(&stopTime);
}


/***********************************************************************
 *                                                                     *
 * Stampa del tempo trascorso.                                         *
 *                                                                     *
 ***********************************************************************/
void printElapsedTime(){
	printf("\nstopTime: %s\n", ctime(&stopTime));
	printf("Tempo totale: %.0f secondi.\n", difftime(stopTime, startTime));	
}


/***********************************************************************
 *                                                                     *
 * Stampa del tempo trascorso.       enrico                            *
 *                                                                     *
 ***********************************************************************/
double printElapsedTime2(){
	printf("\nstopTime: %s\n", ctime(&stopTime));
//	fine = difftime(stopTime, startTime);
	printf("Tempo totale: %.0f secondi.\n", difftime(stopTime, startTime));	
	return (difftime(stopTime, startTime));
}

/***********************************************************************
 *                                                                     *
 * Stampa del tempo trascorso.       pss	                           *
 *                                                                     *
 ***********************************************************************/

double getElapsedTime() {
	ctime(&stopTime);
	return (difftime(stopTime, startTime));
}


/***********************************************************************
 *                                                                     *
 * Determina se e' stato raggiunto il time limit.                      *
 *                                                                     *
 ***********************************************************************/
bool isTimeLimit(){
	time(&tempTime);
	if(difftime(tempTime, startTime) > timeLimit){

#ifdef print_original_logs_vcph
		puts("\n- - -> Raggiunto Time Limit.");
#endif
				
#ifdef print_original_logs_vcph
		//////////////////////////porcheria/////////////////////////
		FILE* fp; 
		//std::cout << "PRINTING TO FILE_: " << instanceName << std::endl;
		char name[200];
		strcpy(name, instanceName.c_str());


		 /* apertura del file in scrittura */
		 fp = fopen ("risultati_gen.txt","a+");
		 fprintf(fp,"instanceName = %s  KColorNumber = %d  random_seed = %d  !!!TIME_LIMIT!!!\n", name, KColorNumber,set_seed);
		 fclose (fp);

		///////////////////////////////////////////////////////////////////
#endif

		return true;
	}
	return false;
}


/***********************************************************************
 *                                                                     *
 * Inizializzazione del generatore di numeri random.                   *
 *                                                                     *
 ***********************************************************************/
void initializeRandomGenerator(){
//	srand(RANDOM_SEED);
	srand(set_seed);
}


/***********************************************************************
 *                                                                     *
 * Restituisce un numero random nell'intervallo [0, "range"[           *
 * (estremo finale escluso).                                           *
 *                                                                     *
 ***********************************************************************/
int getRandomNumber(int range){
	double r,x;
	int y;
	if(range<1){
		printf("getRandomNumber: chiamata non valida con parametro \"range\" = %d.\n", range);
		exit(1);
	}
	
	r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );   /* r is a random floating point value in the range [0,1) {including 0, not including 1}. Note we must convert rand() and/or RAND_MAX+1 to floating point values to avoid integer division. In addition, Sean Scanlon pointed out the possibility that RAND_MAX may be the largest positive integer the architecture can represent, so (RAND_MAX+1) may result in an overflow, or more likely the value will end up being the largest negative integer the architecture can represent, so to avoid this we convert RAND_MAX and 1 to doubles before adding. */ 
	x = (r * range);                /* x is a random floating point value in the range [0,range) {including 0, not including range}. */ 
	y = (int) x;                  /* y is a random integer in the range [0,M) {including 0, not including M}. If M is an integer then the range is [0,M-1] {inclusive} */ 
	
	return y;
}


/*Enrico 12.10.04****************************************************
*																	*
*    Stampa su file la soluzione feasible                           *
*                                                                   *
*********************************************************************/

void UTL_BlankToZero(char *buf)  // ripulisce buffer dai caratteri vuoti by A.Lodi
{
   char *p;
   p = buf;
   while (*p != '\0') {
         if ( *p == ' ') *p = '0';
         p++;
   }
}


/*PSS 03.11.21****************************************************
*																	*
*   Log to file the feasible solution							    *
*                                                                   *
*********************************************************************/

void print_file(std::string instance_name, int *solution) {

	int i;
	char  buf[15];

	/* metto in buf k_target, iterazionu di tabu e dim.popolazione */
	sprintf(buf, "%3d %7d %3d", KColorNumber, tabuSearchIterations, populationSize);
	UTL_BlankToZero(buf);

	std::ofstream of(all_info_VCPh.log_detailed_results_filename, std::ofstream::app);
	of << instance_name << "\t" << buf << std::endl;

	for (i = 0; i < vertexNumber; i++) {
		of << solution[i] << " ";
	}

	of << "[" << vertexNumber << "]";
	of << std::endl;
	of.close();
}

/*PSS 03.11.21****************************************************
*																	*
*   sol2vector														*
*                                                                   *
*********************************************************************/
void sol2vector(int* col, size_t size, std::vector<int>& vcol) {
	vcol.clear();
	for (int i = 0; i < size; i++) {
		vcol.push_back(col[i]);
	}
}

/********************************************************************/

/* Enrico 12.10.04  ****************************************
*                                                          *
*  stampa su file la soluzione fesible                     *
*                                                          *
************************************************************/

//void print_file(int *solution) {			//ORIGINAL CODE
//	
//	/* percorso nel file system */
//	char* pathOutputFile;
//	
//
//	int i;
//	FILE* fp;
//	char   buf[15];
//	
//	/* metto in buf k_target, iterazionu di tabu e dim.popolazione */ 
//	sprintf(buf, "%3d_%7d_%3d",KColorNumber, tabuSearchIterations, populationSize);
//	UTL_BlankToZero(buf);
//
//	/* costruisco il path del file dell'istanza */
//	pathOutputFile=(char*)calloc( strlen(PATH_OUTPUT) + strlen(PATH_SEPARATOR) +
//							     strlen(instanceName) + strlen(buf) + 1, sizeof(char) );
//	
//	
//	if(pathOutputFile==NULL){
//		puts("openFile: out of memory");
//		exit(1);
//	}
//
//	pathOutputFile[0]=NULL;
//	strcat(pathOutputFile, PATH_OUTPUT);
//	strcat(pathOutputFile, PATH_SEPARATOR);
//	strcat(pathOutputFile, buf);
//	strcat(pathOutputFile, instanceName);
//		
//
//		/* apertura del file in scrittura */
//	if( (fp=fopen(pathOutputFile, "a")) == NULL ){
//		printf("File non esistente: %s.\n", pathOutputFile);
//		exit(2);
//	}
//
//	free(pathOutputFile);
//
//	for (i=0;i<vertexNumber;i++)
//		fprintf(fp," %d     %d \n",i+1,solution[i]);
//
//	fclose (fp);
//}






