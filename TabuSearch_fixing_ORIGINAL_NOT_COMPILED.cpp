/*
 * Algoritmo di Tabu Search.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>		//per INT_MAX
#include "CommonData.h"
#include "Utility.h"


/* numero di iterazioni */
int tabuSearchIterations;

/* lunghezza della lista tabu */
int tabuTenure;

/* parametri per il calcolo del tabuTenure */
double parameterAlpha;
int parameterRand;
int parameterConst;


/* dichiarazione delle procedure utilizzate dall'algoritmo */
void update(int minScore, int colorClass);
void selectColor();
void scoreColor();
void selectNextVertexToColor();
boolean isTabu(int colorToQuery);
void initialize();
void freeMemory();
void allocateMemory();


/* vettore contenente la colorazione attuale (colore 0 = vertice scolorato) */
int* actualColoring=NULL;

/* vettore degli score dei colori */
int* scoreColorVector=NULL;

/* numero di vertici non colorati */
int uncoloredNumber;

/* insieme di vertici non colorati */
int* uncoloredVertexSet=NULL;

/* numero temporaneo di vertici non colorati */
int tempUncoloredNumber;

/* insieme temporaneo di vertici non colorati */
int* tempUncoloredVertexSet=NULL;

/* insieme temporaneo di vertici per costruire l'insieme di vertici scolorati */
int* tempSet=NULL;

/* valore della soluzione attuale */
int actualSolutionValue;

/* valore della migliore configurazione di coloring trovata */
int bestColoringValue;

/* migliore configurazione di coloring trovata */
int* bestColoring=NULL;

/* vertice da colorare */
int vertexToColor;

/* lista Tabu implementata da una matrice che memorizza il numero della
   iterazione a partire dalla quale la mossa non viene piu' considerata
   tabu */
int *tabuListTable=NULL;

/* iterazione corrente del Tabu Search */
int currentIter;

/* numero minimo di vertici non colorati */
int minUncoloredNumber;

/*Giuseppe Caselli 14-01-05*/
/* vettore che per ogni vertice contiene un 1 se appartiene alla clique
   0 altrimenti*/
  int cliqueVertexSet[]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1, /* non voglio il vettore 2 insieme al 3 */
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/* Vettore contenete per ogni classe di colore il vertice della clique
    o uno 0 se non tale classe non contiene un vertice della clique*/
  int* cliqueVertexClass=NULL;
/* Flag posto a 1 se il vertice da colorare appartiene alla clique, 0 altrimanti */
  int flag_clique;



/***********************************************************************
 *                                                                     *
 * Allocazione di memoria per le strutture dati.                       *
 *                                                                     *
 ***********************************************************************/
void allocateMemory(){
	if(actualColoring==NULL)
		actualColoring=(int*)calloc(vertexNumber, sizeof(int));
	if(scoreColorVector==NULL)
		scoreColorVector=(int*)calloc(KColorNumber+1, sizeof(int));
	if(uncoloredVertexSet==NULL)
		uncoloredVertexSet=(int*)calloc(vertexNumber, sizeof(int));
	if(tempUncoloredVertexSet==NULL)
		tempUncoloredVertexSet=(int*)calloc(vertexNumber, sizeof(int));
	if(tempSet==NULL)
		tempSet=(int*)calloc(vertexNumber, sizeof(int));
	if(bestColoring==NULL)
		bestColoring=(int*)calloc(vertexNumber, sizeof(int));
	if(tabuListTable==NULL)
		tabuListTable=(int*)calloc(vertexNumber*(KColorNumber+1), sizeof(int));
	/* Giuseppe 14-01-05 */
//	if(cliqueVertexSet==NULL)
//		cliqueVertexSet=(int*)calloc(vertexNumber, sizeof(int));
	if(cliqueVertexClass==NULL)
		cliqueVertexClass=(int*)calloc(KColorNumber+1, sizeof(int));
}



/***********************************************************************
 *                                                                     *
 * Deallocazione di memoria delle strutture dati.                      *
 *                                                                     *
 ***********************************************************************/
void freeMemory(){
	free(actualColoring);
	free(scoreColorVector);
	free(uncoloredVertexSet);
	free(tempUncoloredVertexSet);
	free(tempSet);
	free(bestColoring);
	/*Giuseppe 14-01-05 */
	free(cliqueVertexSet);
	free(cliqueVertexClass);
}



/***********************************************************************
 *                                                                     *
 * Inizializzazione delle strutture dati per il Tabu Search.           *
 *                                                                     *
 ***********************************************************************/
void initialize(){

	/* indice generico */
	int i;


	actualSolutionValue=0;

	bestColoringValue=INT_MAX;

	/* inizializzazione dell'insieme dei vertici non colorati e
	   calcolo del valore della soluzione come somma del grado
	   (= numero dei lati incidenti) dei vertici non colorati */
	uncoloredNumber=0;
	for(i=0; i<vertexNumber; i++){
		if(actualColoring[i]==0){
			actualSolutionValue += adjacentNumberVector[i];
			uncoloredVertexSet[uncoloredNumber]=i;
			uncoloredNumber++;
		}
	}//for

	if(uncoloredNumber==0){
		puts("TabuSearch (initialize): il coloring passato al Tabu Search e' gia' ammissibile.");
		exit(1);
	}

	minUncoloredNumber=uncoloredNumber;

	/* inizializzazione lista Tabu */
	if(parameterRand>0)
		tabuTenure=getRandomNumber(parameterRand)+(int)(parameterAlpha*uncoloredNumber)+parameterConst;
	else
		tabuTenure=(int)(parameterAlpha*uncoloredNumber)+parameterConst;
	for(i=0; i<vertexNumber*(KColorNumber+1); i++)
		tabuListTable[i]=0;

}//initialize




/***********************************************************************
 *                                                                     *
 * Ritorna TRUE se la coppia (vertexToColor, colorToQuery) e' tabu.    *
 *                                                                     *
 ***********************************************************************/
boolean isTabu(int colorToQuery){

	if(currentIter < tabuListTable[vertexToColor*(KColorNumber+1) + colorToQuery])
		   return TRUE;

	return FALSE;

}//isTabu



/***************************************************************
* Giuseppe Caselli 14-01-2005                                  *
* Ritorna TRUE se è possibile assegnare vertexToColor alla     *
* Classe di colore i. Se il vertice da colorare è un vertice   *
* della clique allora non può essere inserito in una classe di *
* colore che contiene già un vertice della clique.             *
* Se il vertice da colorare non appartiene alla clique e la    *
* classe di colore i contiene un vertice della clique allora   *
* essi non devono essere adiacenti, altrimenti in fase di      *
* aggiornamento il vertice della clique verrebbe rimosso.      *
*                                                              *
****************************************************************/
boolean isAcceptable(int i) /* i: classe di colore*/
{   /* indici generici */
	int j, adjacent;
	flag_clique=0;
	/*Verifico se il vertice da colorare appartiene alla clique*/
	
	if(cliqueVertexSet[vertexToColor]==1) /* int *cliqueVertexSet: per ogni vertice contiene 1 se appartiene alla clique 0 altrimenti*/
	{   flag_clique=1;
		/*Se la classe non contiene un vertice della clique allora posso valutare l'assegnamento
		uso un vettore int *cliqueVertexClass: per ogni classe contiene 0 se non ha un vertice della clique, altrimenti il numero corrispondente al vertice.*/
		if(cliqueVertexClass[i]==0)
			return TRUE;
		return FALSE;
	} else /*il vertice da colorare non è un vertice della clique */
		if (cliqueVertexClass[i] != 0)/* se la classe da colorare ha un vertice della clique*/
		{
			/*Devo verificare se il vertice  della clique contenuto in tale classe è adiacente a vertexToColor*/
			
			for (j=pointerVector[cliqueVertexClass[i]]; j<pointerVector[cliqueVertexClass[i]+1]; j++){
			adjacent = adjacentVector[j];
			if (adjacent == vertexToColor)
				return FALSE; /* non posso assegnare quel vertice alla classe di colore i. */
			}//for
		return TRUE;
			
		}else
			return TRUE;
	
}//isCompatible



/***********************************************************************
 *                                                                     *
 * Scelta random del prossimo vertice (non colorato) da colorare.      *
 *                                                                     *
 ***********************************************************************/
void selectNextVertexToColor(){

	/* numero random per individuare un vertice nell'insieme di quelli non colorati */
	int randomNumber;

	/* ottengo un numero random nel range [0, uncoloredNumber[ */
	randomNumber=getRandomNumber(uncoloredNumber);

    vertexToColor=uncoloredVertexSet[randomNumber];

}//selectNextVertexToColor




/***********************************************************************
 *                                                                     *
 * Valutazione di uno score per ogni colore: ad ogni colore "i" si     *
 * associa la sommatoria del grado (= numero lati incidenti) dei       *
 * vertici di colore "i" che sono adiacenti al vertice scelto da       *
 * colorare.                                                           *
 * Alcuni di tali adiacenti, a seguito della scelta di un colore per   *
 * il vertice, verranno scolorati e l'obiettivo e' minimizzare il      *
 * grado dei vertici non colorati.                                     *
 *                                                                     *
 ***********************************************************************/
void scoreColor(){

	/* indici generici */
	int i,adjacent;

	for(i=1; i<=KColorNumber; i++) scoreColorVector[i]=0;

	for(i=pointerVector[vertexToColor]; i<pointerVector[vertexToColor+1]; i++){
		adjacent=adjacentVector[i];
		scoreColorVector[actualColoring[adjacent]] += adjacentNumberVector[adjacent];
		//se actualColoring[adjacent] e' 0, viene aggiornata la posizione 0 del
		//vettore, che comunque viene ignorata
	}

}//scoreColor




/***********************************************************************
 *                                                                     *
 * Scelta del colore da assegnare al vertice da colorare, in base al   *
 * criterio di minimizzazione dello score calcolato e al criterio di   *
 * "aspirazione" del Tabu Search (una mossa che porta ad una           *
 * configurazione di colore di valore inferiore alla migliore          *
 * soluzione trovata viene sempre accettata anche se tabu).            *
 *                                                                     *
 ***********************************************************************/
void selectColor(){

	/* indici generici */
	int i, count, position;

	/* minimo score trovato */
	int minScore;

	/* classe di colore scelta */
	int colorClass;


	minScore=INT_MAX;
	colorClass=-1;

	for(i=1; i<=KColorNumber; i++){// Giuseppe modificato l'if
		if( (scoreColorVector[i]<minScore) && (isAcceptable(i)==TRUE) && 
			( (isTabu(i)==FALSE) ||
					(actualSolutionValue - adjacentNumberVector[vertexToColor] +
					scoreColorVector[i] < bestColoringValue) ) //criterio di aspirazione
			){
				minScore=scoreColorVector[i];
				colorClass=i;
			}//if ciclo1
	}//for


	/* nel caso in cui non sia possibile assegnare un colore, si passa a considerare
	   il vertice successivo nell'insieme dei vertici non colorati */
	count=1; //un vertice scolorato e' gia' stato esaminato
	while(colorClass<0 && count<uncoloredNumber){ //finche' trovo un colore o ho esaminato tutti i vertici non colorati
		//trovo la posizione del vertice corrente, per passare al successivo
		for(i=0; i<uncoloredNumber; i++)
			if(uncoloredVertexSet[i]==vertexToColor) position=i;
		position++;
		if(position==uncoloredNumber) position=0;
		vertexToColor=uncoloredVertexSet[position];
		scoreColor();
		minScore=INT_MAX;
		colorClass=-1;
		for(i=1; i<=KColorNumber; i++){// Giuseppe modificato l'if
			if( (scoreColorVector[i]<minScore) && (isAcceptable(i)==TRUE)&& 
				( (isTabu(i)==FALSE) ||
				(actualSolutionValue - adjacentNumberVector[vertexToColor] +
				scoreColorVector[i] < bestColoringValue) ) //criterio di aspirazione
			){
				minScore=scoreColorVector[i];
				colorClass=i;
			}//if
		}//for  ciclo2
		count++;
	}

	/* per evitare che nessun colore sia ammissibile, viene scelto comunque
	   il colore che determina lo score minore (per l'ultimo vertice esaminato) */
	if(colorClass<0){
		for(i=1; i<=KColorNumber; i++)// Giuseppe modificato l'if
			if((scoreColorVector[i]<minScore) && (isAcceptable(i)==TRUE)) {
				minScore=scoreColorVector[i];
				colorClass=i;
			}
	}//if ciclo3
	

	/* aggiornamento della configurazione di coloring */
	update(minScore, colorClass);

}//selectColor



/***********************************************************************
 *                                                                     *
 * Aggiornamento della configurazione di coloring in base al vertice   *
 * e al colore scelto.                                                 *
 * "minScore" e' lo score corrispondente al colore scelto.             *
 * "colorClass" e' il colore da assegnare al vertice.                  *
 *                                                                     *
 ***********************************************************************/
void update(int minScore, int colorClass){

	/* indici generici */
	int i, i1, i2, t, a;

	/* adiacente al vertice scelto */
	int adjacent;

	/* colorazione del vertice scelto */
	actualColoring[vertexToColor]=colorClass;

	/* Giuseppe Caselli 14-01-05 */
	/* Se il vertice appena colorato appartiene alla clique devo aggiornare
	   il vettore cliqueVertexClass inserendo tale vertice nella posizione
	   corrispondente alla classe "colorClass" */
	if (flag_clique==1){
		cliqueVertexClass[colorClass] = vertexToColor;
		flag_clique=0;
	}


	/* scolorazione degli adiacenti aventi il colore scelto */
	tempUncoloredNumber=0;
	for(i=pointerVector[vertexToColor]; i<pointerVector[vertexToColor+1]; i++){
		adjacent=adjacentVector[i];
		if(actualColoring[adjacent]==colorClass){
			actualColoring[adjacent]=0;
			if (actualColoring[24]==2) {

				a=1;
			}

			if (actualColoring[24]==0) {

				a=1;
			}
		

			/* vettore temporaneo di vertici scolorati */
			tempUncoloredVertexSet[tempUncoloredNumber]=adjacent;
			tempUncoloredNumber++;
		}
	}

	/* aggiornamento del valore della soluzione */
	actualSolutionValue = actualSolutionValue - adjacentNumberVector[vertexToColor] + minScore;

	/* in accordo con l'articolo "HEA", per generare una popolazione il piu'
	   possibile diversificata, si aggiorna la migliore soluzione anche nel
	   caso in cui se ne trovi un'altra con lo stesso valore ( <= ), in modo da
	   ritornare quella trovata piu' di recente, e quindi ragionevolmente piu'
	   lontana dalla configurazione iniziale */
	if(actualSolutionValue <= bestColoringValue){
		bestColoringValue=actualSolutionValue;
		for(i=0; i<vertexNumber; i++) bestColoring[i]=actualColoring[i];
	}

	/* aggiornamento dell'insieme dei vertici non colorati: merge dei
	   due insiemi (gia' ordinati) uncoloredVertexSet e tempUncoloredVertexSet,
	   trascurando il vertice appena colorato */
	t=uncoloredNumber;
	uncoloredNumber=0;
	i1=0;
	i2=0;
	while(i1<t || i2<tempUncoloredNumber){
		if(i2==tempUncoloredNumber){ //l'insieme 2 e' vuoto
			while(i1<t){
				if(uncoloredVertexSet[i1]==vertexToColor){
					i1++;
				}else{
					tempSet[uncoloredNumber]=uncoloredVertexSet[i1];
					uncoloredNumber++;
					i1++;
				}
			}
			continue;
		}
		if(i1==t){ //l'insieme 1 e' vuoto
			while(i2<tempUncoloredNumber){
				tempSet[uncoloredNumber]=tempUncoloredVertexSet[i2];
				uncoloredNumber++;
				i2++;
			}
			continue;
		}
		if(uncoloredVertexSet[i1]<tempUncoloredVertexSet[i2]){
			if(uncoloredVertexSet[i1]==vertexToColor){
				i1++;
			}else{
				tempSet[uncoloredNumber]=uncoloredVertexSet[i1];
				uncoloredNumber++;
				i1++;
			}
		}else{
			tempSet[uncoloredNumber]=tempUncoloredVertexSet[i2];
			uncoloredNumber++;
			i2++;
		}
	}
	for(i=0; i<uncoloredNumber; i++) uncoloredVertexSet[i]=tempSet[i];

	/* aggiornamento del minimo numero di vertici non colorati */
	if(uncoloredNumber < minUncoloredNumber) minUncoloredNumber=uncoloredNumber;

	/* aggiornamento della lista tabu */
	if(parameterRand>0)
		tabuTenure=getRandomNumber(parameterRand)+(int)(parameterAlpha*uncoloredNumber)+parameterConst;
	else
		tabuTenure=(int)(parameterAlpha*uncoloredNumber)+parameterConst;

	tabuListTable[vertexToColor*(KColorNumber+1) + colorClass]=currentIter+tabuTenure+1;
									//numero dell'iterazione in cui non sara' piu' tabu

}//update




/***********************************************************************
 *                                                                     *
 * Tabu Search.                                                        *
 * "coloring" e' parametro di ingresso-uscita: la configurazione       *
 * iniziale da migliorare viene aggiornata con la migliore             *
 * colorazione trovata. Ritorna anche il corrispondente valore         *
 * di tale soluzione.                                                  *
 *                                                                     *
 ***********************************************************************/
int tabuSearch(int* coloring){

	/* indice generico */
	int i;

	/* allocazione di memoria per le strutture dati */
	allocateMemory();

	/* copia di lavoro della configurazione di coloring */
	for(i=0; i<vertexNumber; i++) actualColoring[i]=coloring[i];

	initialize();

	/* ciclo principale */
	for(currentIter=0; currentIter<tabuSearchIterations; currentIter++){

		/* selezione vertice da colorare tra quelli non colorati */
		selectNextVertexToColor();

 		/* calcolo dello score corrispondente ad ogni colore */
		scoreColor();

		/* selezione del colore per il nuovo vertice da colorare*/
   		selectColor();

   		/* se viene individuata una soluzione ammissibile, si termina il ciclo */
		if(bestColoringValue == 0) break;

	}//for

	/* la migliore configurazione trovata viene copiata nel vettore di
	   ingresso-uscita */
	for(i=0; i<vertexNumber; i++) coloring[i]=bestColoring[i];

	/* ritorna il valore corrispondente della migliore soluzione trovata */
	return bestColoringValue;

}//tabuSearch
