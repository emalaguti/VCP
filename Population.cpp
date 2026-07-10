/*
 * Procedure per la gestione della popolazione ("pool" di
 * soluzioni attive).
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "CommonData.h"
#include "Population.h"
#include "GreedyColoring.h"
#include "TabuSearch.h"
#include "Utility.h"
#include "CrossoverOperator.h"
#include "CheckSolution.h"


/* dimensione della popolazione */
int populationSize;


/* limite di tempo per le generazioni */
int timeLimit;


/* popolazione: vettore di puntatori a soluzioni (coloring non ammissibili) */
int** population;


/* vettore contenente i valori delle soluzioni corrispondenti
   ad ogni configurazione (parent) contenuta nella popolazione
   (utilizzato per efficienza) */
int* parentValueVector;


/* vettore contenente, per ogni soluzione nel pool, il numero
   di vertici non colorati (utilizzato per efficienza) */
int* parentUncoloredNumberVector;


/* vettore contenente i contatori del numero di volte in cui un vertice e' scolorato */
unsigned int* uncoloredTimes;


/* indica che nessuna soluzione e' stata sostituita dal meccanismo che
   preserva la diversita' del pool */
#define NOT_REPLACED -1


/* indica quale soluzione e' stata sostituita dal meccanismo che preserva
   la diversita' del pool  */
int replacedParent=NOT_REPLACED;


/* dichiarazione delle procedure utilizzate dall'algoritmo */
int getUncoloredNumber(int *coloring);
int getSolutionValue(int* coloring);
void replaceExistingSolution(int *coloring);
void greedyPriorityColoring(int* coloring);
void diversityFunction();


/*cross over data estructures (improved memory allocation)-pss  */
extern int* s1;
extern int* s2;
extern int* colorClass;

/***********************************************************************
 *                                                                     *
 * Inizializzazione di un pool di configurazioni di coloring.          *
 *                                                                     *
 ***********************************************************************/
void initPopulation(){

	/* indici generici */
	int i, j;

	/* puntatore ad una configurazione di coloring */
	int* coloring;

	/* indica se e' stata trovata una soluzione ammissibile */
	bool found = false;
	
	
	/* creazione del vettore popolazione */
	population=(int**)calloc(populationSize, sizeof(int*));
	for(i=0; i<populationSize; i++) population[i]=NULL;

	/* creazione del vettore contenente gli score dei genitori */
	parentValueVector=(int*)calloc(populationSize, sizeof(int));

	/* creazione del vettore contenente il numero di vertici scolorati dei parent */
	parentUncoloredNumberVector=(int*)calloc(populationSize, sizeof(int));

	/* creazione del vettore dei contatori del numero di volte in cui un vertice
	   e' scolorato */
	uncoloredTimes=(unsigned int*)calloc(vertexNumber, sizeof(int));
	for(i=0; i<vertexNumber; i++) uncoloredTimes[i]=0;

	/* creazione delle configurazioni di coloring */
	for(i=0; i<populationSize; i++){

		/* creazione di ogni nuovo vettore contenente il coloring */
		coloring=(int*)calloc(vertexNumber, sizeof(int));

		/* inizializzazione del coloring */
		for(j=0; j<vertexNumber; j++) coloring[j]=0;

		if(i%3==0) randomSequentialColoring(coloring);
		else if(i%3==1) randomSaturationGreedy(coloring);
		else tabuSearch(coloring);

		/* controllo se e' stata trovata una soluzione ammissibile:
		   in caso affermativo fermo l'applicazione */
		found=true;
		for(j=0; j<vertexNumber; j++) if(coloring[j]==0){found=false; break;}
		if (found == true) {
			if(feasibleSolutionFound(coloring, "GreedyColoring")==-1) { all_info_VCPh.status = info_VCPh::ERR; }
			sol2vector(coloring, vertexNumber, all_info_VCPh.sol);
			all_info_VCPh.is_sol_found = true;
			break;
		}

		/* ogni nuova configurazione creata viene immediatamente migliorata dal
	       Tabu Search, prima di essere inserita nella popolazione */
		tabuSearch(coloring);

		/* controllo se Tabu Search ha trovato una soluzione ammissibile:
		   in caso affermativo fermo l'applicazione */
		found=true;
		for(j=0; j<vertexNumber; j++) if(coloring[j]==0){found=false; break;}
		if (found == true) {
			if(feasibleSolutionFound(coloring, "TabuSearch (durante InitPopulation)") ==-1){ all_info_VCPh.status = info_VCPh::ERR; }
			sol2vector(coloring, vertexNumber, all_info_VCPh.sol);
			all_info_VCPh.is_sol_found = true;
			break;
		}

		/* inserimento della configurazione nella popolazione */
		population[i]=coloring;

		/* calcolo del valore della soluzione corrispondente al coloring */
		parentValueVector[i]=getSolutionValue(coloring);

		/* calcolo del numero di vertici scolorati del coloring */
		parentUncoloredNumberVector[i]=getUncoloredNumber(coloring);

		/* aggiornamento del vettore dei contatori dei vertici scolorati */
		for(j=0; j<vertexNumber; j++) if(coloring[j]==0) uncoloredTimes[j]++;

	}//for
	
}//initPopulation



/***********************************************************************
 *                                                                     *
 * Deallocazione della memoria utilizzata dalla popolazione.           *
 *                                                                     *
 ***********************************************************************/
void freePopulation(){

	/* indice generico */
	int i;

	/* deallocazione di ogni coloring contenuto nella popolazione */
	for(i=0; i<populationSize; i++)
		if (population[i] != NULL) {
			free(population[i]);			
		}

	/* deallocazione del vettore popolazione */
	free(population);  

	/* deallocazione del vettore contenente i valori delle soluzioni parent */
	free(parentValueVector); 

	/* deallocazione del vettore contenente il numero di vertici scolorati
	   dei parent */
	free(parentUncoloredNumberVector); 

	/* deallocazione del vettore dei contatori del numero di volte in cui un vertice
	   e' scolorato */
	free(uncoloredTimes); 

}//freePopulation




/***********************************************************************
 *                                                                     *
 * Scelta random di due genitori contenuti nella popolazione.          *
 * I parametri di ritorno sono "indexParentS1" e "indexParentS2", che  *
 * rappresentano gli indici dei genitori scelti all'interno del        *
 * vettore della popolazione.                                          *
 *                                                                     *
 ***********************************************************************/
void chooseParents(int* indexParentS1, int* indexParentS2){

	/* determino il primo indice del genitore S1 ottenendo un numero
	   random nel range [0 , populationSize[ */
	*indexParentS1=getRandomNumber(populationSize);

	/* l'indice del genitore S2 deve essere differente da quello di S1 */
	do{
		*indexParentS2=getRandomNumber(populationSize);
	}while(*indexParentS1 == *indexParentS2);

}//chooseParents




/***********************************************************************
 *                                                                     *
 * Aggiornamento della popolazione tramite l'inserimento della nuova   *
 * configurazione creata (e migliorata dal Tabu Search), che viene     *
 * sostituita al peggiore dei due parent che l'hanno generata          *
 * (cioe', quello con il maggiore valore della soluzione), tranne nel  *
 * caso in cui intervenga il meccanismo che preserva la diversita'     *
 * del pool di soluzioni.                                              *
 * In ingresso si hanno gli indici dei due parent all'interno della    *
 * popolazione e il coloring dell'offspring.                           *
 *                                                                     *
 ***********************************************************************/
void updatePopulation(int indexParentS1, int indexParentS2, int* offspring){

	/* indice generico */
	int i;

	/* valore della soluzione dei parent */
	int parentS1Value;
	int parentS2Value;

	/* indice del parent da sostituire */
	int selectedParentIndex;

	/* vettore parent da sostituire */
	int* selectedParent;


	parentS1Value=parentValueVector[indexParentS1];
	parentS2Value=parentValueVector[indexParentS2];

	/* scelta del parent da sostituire */
	if(parentS1Value >= parentS2Value){			   //parentS1 da sostituire
		selectedParentIndex=indexParentS1;
	}else{										   //parentS2 da sostituire
		selectedParentIndex=indexParentS2;
	}

	/* caso in cui la soluzione da sostituire sia proprio quella appena modificata
	   dal meccanismo che preserva la diversita' del pool */
	if(replacedParent != NOT_REPLACED){
		if(replacedParent==indexParentS1) selectedParentIndex=indexParentS2;
		else if(replacedParent==indexParentS2) selectedParentIndex=indexParentS1;
		replacedParent=NOT_REPLACED; //viene reinizializzato
	}

	selectedParent=population[selectedParentIndex];

	/* sostituizione del genitore selezionato con l'offspring */
	for(i=0; i<vertexNumber; i++) selectedParent[i]=offspring[i];

	/* aggiornamento del vettore contenente i valori delle soluzioni */
	parentValueVector[selectedParentIndex]=getSolutionValue(offspring);

	/* aggiornamento del vettore contenente il numero di vertici scolorati
	   delle soluzioni */
	parentUncoloredNumberVector[selectedParentIndex]=getUncoloredNumber(offspring);

	/* aggiornamento del vettore dei contatori dei vertici scolorati */
	for(i=0; i<vertexNumber; i++) if(offspring[i]==0) uncoloredTimes[i]++;

}//updatePopulation




/***********************************************************************
 *                                                                     *
 * Ritorna il valore della soluzione corrispondente alla               *
 * configurazione di coloring passata in ingresso, calcolando la       *
 * sommatoria del grado (= numero lati incidenti) dei vertici          *
 * non colorati.                                                       *
 *                                                                     *
 ***********************************************************************/
int getSolutionValue(int* coloring){

	/* indice generico */
	int i;

	/* valore della soluzione da ritornare */
	int solutionValue;

	solutionValue=0;

	for(i=0; i<vertexNumber; i++)
		if(coloring[i]==0) solutionValue += adjacentNumberVector[i];

	return solutionValue;

}//getSolutionValue



/***********************************************************************
 *                                                                     *
 * Ritorna il numero di vertici non colorati.                          *
 *                                                                     *
 ***********************************************************************/
int getUncoloredNumber(int *coloring){
	
	/* indice generico */
	int i;

	/* numero di vertici non colorati */
	int uncoloredNumber;

	uncoloredNumber=0;
	for(i=0; i<vertexNumber; i++) if(coloring[i]==0) uncoloredNumber++;

	return uncoloredNumber;
}//getUncoloredNumber



/***********************************************************************
 *                                                                     *
 * Ciclo di generazione.                                               *
 *                                                                     *
 ***********************************************************************/
void generationCycle(){

	/* indice generico */
	int i;

	/* indica se e' stata trovata una soluzione ammissibile */
	bool found=false;

	/* contatore delle generazioni */
	int cycle;

	/* indici dei genitori scelti */
	int indexParentS1;
	int indexParentS2;

	/* figlio creato */
	int* offspring ;

	/* numero medio di vertici scolorati nel pool */
	int averageUncolored;


	offspring=(int*)calloc(vertexNumber, sizeof(int));

	////////////////
	//allocation for crossover-pss 
	s1=(int*)calloc(vertexNumber, sizeof(int));								
	s2=(int*)calloc(vertexNumber, sizeof(int));	
	colorClass = (int*)calloc(KColorNumber + 1, sizeof(int));				 
	

	/* ciclo di generazioni */
	for(cycle=0; isTimeLimit()==false; cycle++){

		/* scelta dei genitori */
		chooseParents(&indexParentS1, &indexParentS2);

		/* operatore di crossover */
		crossover(population[indexParentS1], population[indexParentS2], offspring);

		/* controllo se Crossover ha trovato una soluzione ammissibile:
		   in caso affermativo fermo l'applicazione */
		found=true;
		for(i=0; i<vertexNumber; i++) if(offspring[i]==0){found=false; break;}
		if(found==true){
#ifdef print_original_logs_vcph
			printf("\nNumero totale di generazioni: %d\n", cycle);
#endif
			if (feasibleSolutionFound(offspring, "Crossover") == -1) { all_info_VCPh.status = info_VCPh::ERR; }
			sol2vector(offspring, vertexNumber, all_info_VCPh.sol);
			all_info_VCPh.is_sol_found = true;			
			break;
		}

		/* miglioramento del figlio tramite la procedura di Tabu Search */
		tabuSearch(offspring);

		/* controllo se Tabu Search ha trovato una soluzione ammissibile:
		   in caso affermativo fermo l'applicazione */
		found=true;
		for(i=0; i<vertexNumber; i++) if(offspring[i]==0){found=false; break;}
		if(found==true){
#ifdef print_original_logs_vcph
			printf("\nNumero totale di generazioni: %d\n", cycle);
#endif
			if ( feasibleSolutionFound(offspring, "TabuSearch")) { all_info_VCPh.status = info_VCPh::ERR; }		

			sol2vector(offspring, vertexNumber, all_info_VCPh.sol);
			all_info_VCPh.is_sol_found = true;			
			break;
		}

		
		/* la procedura per preservare la diversita' del pool viene
		   applicata con una probabilita' inversamente proporzionale al
		   numero di vertici scolorati nella popolazione */
		averageUncolored=0;
		for(i=0; i<populationSize; i++)
			averageUncolored += parentUncoloredNumberVector[i];
		averageUncolored=averageUncolored/populationSize;
		
		if(getRandomNumber(averageUncolored+1)==0)
			replaceExistingSolution(offspring); /* cosa cosa???


		/* aggiornamento della popolazione con l'inserimento del nuovo figlio,
		   che sostituisce il genitore peggiore */
		updatePopulation(indexParentS1, indexParentS2, offspring);

	}//for

	/* terminazione del ciclo di generazioni senza successo */
#ifdef print_original_logs_vcph
	if (found = false) {
		printf("\nNumero totale di generazioni: %d\n", cycle);
	}
#endif

	/* deallocazione memoria del figlio */
	free(offspring);
	free(colorClass);
	free(s1);
	free(s2);
	
}//generationCycle



/***********************************************************************
 *                                                                     *
 * Procedura che viene applicata per preservare la diversita' del      *
 * pool. Se e' gia' presente una soluzione S che ha lo stesso valore   *
 * di "coloring" e lo stesso numero di vertici scolorati, X viene      *
 * sostituita da una soluzione greedy.                                *
 *                                                                     *
 ***********************************************************************/
void replaceExistingSolution(int *coloring){

	/* indici generici */
	int i, j;

	/* per verificare se e' stata trovata una soluzione greedy ammissibile */
	bool found;

	/* caratteristiche del coloring da controllare */
	int newUncoloredNumber, newSolutionValue;
	
	newUncoloredNumber=getUncoloredNumber(coloring);
	newSolutionValue=getSolutionValue(coloring);

	for(i=0; i<populationSize; i++){
		if(parentValueVector[i] == newSolutionValue &&
			newUncoloredNumber == parentUncoloredNumberVector[i]){
	
			/* la soluzione gia' presente viene sostituita con una
			   greedy che da' priorita' ai vertici maggiormente scolorati */
			greedyPriorityColoring(population[i]);

			/* controllo se e' stata trovata una soluzione ammissibile:
			   in caso affermativo fermo l'applicazione */
			found=true;
			for(j=0; j<vertexNumber; j++) if(population[i][j]==0){found=false; break;}
			if(found==true) feasibleSolutionFound(population[i], "GreedyPriorityColoring");


			parentValueVector[i]=getSolutionValue(population[i]);			
			parentUncoloredNumberVector[i]=getUncoloredNumber(population[i]);

			/* per evitare che update() elimini proprio la soluzione appena sostituita,
			   gli si passa l'indice "i" nella variabile replacedParent */
			replacedParent=i;
		
			break;
		}//if
	}//for
}//replaceExistingSolution



/***********************************************************************
 *                                                                     *
 * Algoritmo greedy che costruisce un coloring dando priorita' ai      *
 * vertici scolorati. Per essere random, prende i vertici scolorati    *
 * a due due dal vettore vertexSequence e li colora scegliendoli       *
 * casualmente.                                                        *
 *                                                                     *
 ***********************************************************************/
void greedyPriorityColoring(int* coloring){

	/* indici generici */
	int i, j, temp;

	/* indica se e' stato assegnato un colore al vertice in esame */
	bool assigned;

	/* vertice scelto */
	int selectedVertex;

	/* indice nel vettore delle priorita' */
	int sequenceIndex;

	/* vettori temporanei */
	static int* uncoloredTimesTemp=NULL;
	static int* vertexSequence=NULL;
	int vertexSequenceNumber;
	int max, next;

	if(uncoloredTimesTemp==NULL)
		uncoloredTimesTemp=(int*)calloc(vertexNumber, sizeof(int));
	if(vertexSequence==NULL)
		vertexSequence=(int*)calloc(vertexNumber, sizeof(int));

	vertexSequenceNumber=0;

	for(j=0; j<vertexNumber; j++) uncoloredTimesTemp[j]=uncoloredTimes[j];
	while(true){
		max=-1;
		for(j=0; j<vertexNumber; j++){
			if(uncoloredTimesTemp[j] != -1 && uncoloredTimesTemp[j] >max){
				max=uncoloredTimesTemp[j];
				next=j;
			}
		}
		if(max==-1)break;
		vertexSequence[vertexSequenceNumber]=next;
		vertexSequenceNumber++;
		uncoloredTimesTemp[next]=-1;
	}


	/* randomizzo la scelta dei vertici, scambiandoli a due a due in successione,
	   con probabilita' del 50% */
	sequenceIndex=0;
	while(sequenceIndex<=(vertexNumber-2)){
		if(getRandomNumber(2)==0){
			temp=vertexSequence[sequenceIndex];
			vertexSequence[sequenceIndex]=vertexSequence[sequenceIndex+1];
			vertexSequence[sequenceIndex+1]=temp;
		}
		sequenceIndex+=2;
	}

	for(i=0; i<vertexNumber; i++) coloring[i]=0;  //invalido il coloring
	sequenceIndex=0;

	while(sequenceIndex < vertexNumber){ //finche' ogni vertice e' stato esaminato

		selectedVertex=vertexSequence[sequenceIndex];

		coloring[selectedVertex]=1;    //si inizia col colore di indice minimo
		do{
			if(coloring[selectedVertex] > KColorNumber){
				coloring[selectedVertex]=0;   //non puo' essere colorato con K colori
				break;                              //passo al vertice successivo
			}
			assigned=true;
			for(j=pointerVector[selectedVertex]; j<pointerVector[selectedVertex+1]; j++){
				if(coloring[selectedVertex]==coloring[adjacentVector[j]]){
					assigned=false;                       //colore in conflitto					
					coloring[selectedVertex]++;     //passo al colore successivo
					break;                   //esco dal for per riesaminare gli adiacenti
				}
			}//for
		}while(assigned==false);

		/* passo al vertice successivo */
		sequenceIndex++;

	}//while

	//PSS-allocation
	free(uncoloredTimesTemp);
	free(vertexSequence);
	uncoloredTimesTemp = NULL;
	vertexSequence = NULL;

}//greedyPriorityColoring




/***********************************************************************
 *                                                                     *
 * Procedura per la stima (offline) della diversita' del pool.         *
 * Confronta le soluzioni a coppie e ne fa una media.                  *
 *                                                                     *
 ***********************************************************************/
void diversityFunction(){

	/* indici generici */
	int i, j, tempColor, tempValue, averageValue;

	/* matrici temporanee per confrontare le soluzioni */
	int* matrix1;
	int* matrix2;

	/* indice delle soluzioni da esaminare */
	int popIndex;

	/* numero di coppie di soluzioni da esaminare */
	int valuesNumber;

	/* vettore contenente il valore del confronto delle coppie di soluzioni */
	int* valuesVector;
	int valuesVectorIndex;

	/*calcolo del numero di coppie */
	valuesNumber=populationSize/2;
	if(valuesNumber*2 < populationSize) valuesNumber++; //dimensione popolazione dispari
	
	/* allocazione memoria */
	matrix1=(int*)calloc(vertexNumber*vertexNumber, sizeof(int));
	matrix2=(int*)calloc(vertexNumber*vertexNumber, sizeof(int));
	valuesVector=(int*)calloc(valuesNumber, sizeof(int));

	popIndex=0;
	valuesVectorIndex=0;
	while(popIndex<populationSize && popIndex+2<=populationSize){

		for(i=0; i<vertexNumber*vertexNumber; i++) matrix1[i]=0;
		for(i=0; i<vertexNumber*vertexNumber; i++) matrix2[i]=0;

		/* costruzione prima matrice */
		for(i=0; i<vertexNumber; i++){
			tempColor=population[popIndex][i];
			for(j=0; j<vertexNumber; j++){
				if(population[popIndex][j]==tempColor)
					matrix1[i*vertexNumber+j]=1;
			}
		}

		/* costruzione seconda matrice */
		for(i=0; i<vertexNumber; i++){
			tempColor=population[popIndex+1][i];
			for(j=0; j<vertexNumber; j++){
				if(population[popIndex+1][j]==tempColor)
					matrix2[i*vertexNumber+j]=1;
			}
		}

		/* confronto (sovrapposizione) matrici */
		tempValue=0;
		for(i=0; i<vertexNumber*vertexNumber; i++){
			if(matrix1[i] != matrix2[i]) tempValue++;
		}

		valuesVector[valuesVectorIndex]=tempValue;
		valuesVectorIndex++;

		popIndex=popIndex+2;
	}

	if(popIndex==populationSize-1){ //l'ultima soluzione viene confrontata con
									//la prima, chiudendo l'anello

		for(i=0; i<vertexNumber*vertexNumber; i++) matrix1[i]=0;
		for(i=0; i<vertexNumber*vertexNumber; i++) matrix2[i]=0;

		/* costruzione prima matrice */
		for(i=0; i<vertexNumber; i++){
			tempColor=population[popIndex][i];
			for(j=0; j<vertexNumber; j++){
				if(population[popIndex][j]==tempColor)
					matrix1[i*vertexNumber+j]=1;
			}
		}

		/* costruzione matrice del primo genitore nella popolazione */
		for(i=0; i<vertexNumber; i++){
			tempColor=population[0][i];
			for(j=0; j<vertexNumber; j++){
				if(population[0][j]==tempColor)
					matrix2[i*vertexNumber+j]=1;
			}
		}

		/* confronto (sovrapposizione) matrici */
		tempValue=0;
		for(i=0; i<vertexNumber*vertexNumber; i++){
			if(matrix1[i] != matrix2[i]) tempValue++;
		}

		valuesVector[valuesVectorIndex]=tempValue;
	}

	tempValue=0;
	for(i=0; i<valuesNumber; i++) tempValue+=valuesVector[i];
	averageValue=tempValue/valuesNumber;

	printf("%d\n", averageValue);

	free(matrix1);
	free(matrix2);
	free(valuesVector);

}//diversityFunction
