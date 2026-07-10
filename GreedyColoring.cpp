/*
 * Algoritmi greedy per il coloring.
 */

#include <stdlib.h>
#include <limits.h>		//per INT_MAX
#include "CommonData.h"
#include "Utility.h"

#define INVALID -1
//per invalidare il coloring


/***********************************************************************
 *                                                                     *
 * Greedy sequenziale che assegna ad ogni vertice il colore            *
 * ammissibile di indice minimo. Le classi di colore sono al piu' K,   *
 * pertanto un vertice che non puo' essere colorato ha colore 0.       *
 * "greedyColoring" e' parametro di ingresso-uscita.                   *
 *                                                                     *
 ***********************************************************************/
void sequentialColoring(int* greedyColoring){

	/* indici generici */
	int i, j;

	/* indica se e' stato assegnato un colore al vertice in esame */
	bool assigned;

	for(i=0; i<vertexNumber; i++) greedyColoring[i]=INVALID;  //invalido il coloring

	for(i=0; i<vertexNumber; i++){
		greedyColoring[i]=1;    //si inizia col colore di indice minimo
		do{
			if(greedyColoring[i] > KColorNumber){
				greedyColoring[i]=0;              //non puo' essere colorato con K colori
				break;                            //passo al vertice successivo
			}
			assigned=true;
			for(j=pointerVector[i]; j<pointerVector[i+1]; j++){
				if(greedyColoring[i]==greedyColoring[adjacentVector[j]]){   //colore in conflitto
					assigned=false;
					greedyColoring[i]++;     //passo al colore successivo
					break;                   //esco dal for per riesaminare gli adiacenti
				}
			}//for
		}while(assigned==false);
	}//for

}//sequentialColoring



/***********************************************************************
 *                                                                     *
 * Greedy sequenziale simile a "sequentialColoring()" ma che permette  *
 * di indicare il vertice da cui iniziare la colorazione.              *
 * "greedyColoring" e' parametro di ingresso-uscita.                   *
 *                                                                     *
 ***********************************************************************/
void sequentialColoringFromVertex(int fromVertex, int* greedyColoring){

	/* indici generici */
	int i, j;

	/* indica se e' stato assegnato un colore al vertice in esame */
	bool assigned;

	for(i=0; i<vertexNumber; i++) greedyColoring[i]=INVALID;  //invalido il coloring

	//la colorazione avviene sequenzialmente in due fasi:

	//per i commenti del codice seguente, vedere "sequentialColoring()"
	#define	coloringPhase														\
		{																		\
			greedyColoring[i]=1;												\
			do{																	\
				if(greedyColoring[i] > KColorNumber){							\
					greedyColoring[i]=0;										\
					break;														\
				}																\
				assigned=true;													\
				for(j=pointerVector[i]; j<pointerVector[i+1]; j++){				\
					if(greedyColoring[i]==greedyColoring[adjacentVector[j]]){   \
						assigned=false;											\
						greedyColoring[i]++;									\
						break;													\
					}															\
				}																\
			}while(assigned==false);											\
		}

	//fase 1: insieme di vertici [fromVertex,vertexNumber[
	for(i=fromVertex; i<vertexNumber; i++)
		coloringPhase

	// fase 2: insieme di vertici [0,fromVertex[
	for(i=0; i<fromVertex; i++)
		coloringPhase

}//sequentialColoringFromVertex




/***********************************************************************
 *                                                                     *
 * Algoritmo greedy di colorazione che permette di ottenere            *
 * colorazioni differenti mediante una scelta random della sequenza    *
 * dei vertici da colorare. E' basato su "sequentialColoring()".       *
 * "greedyColoring" e' parametro di ingresso-uscita.                   *
 *                                                                     *
 ***********************************************************************/
void randomSequentialColoring(int* greedyColoring){

	/* indici generici */
	int i, j;

	/* indica se e' stato assegnato un colore al vertice in esame */
	bool assigned;

	/* insieme di vertici non ancora esaminati */
	int* unexaminedVertexSet;

	/* numero di vertici non ancora esaminati */
	int unexaminedNumber;

	/* numero random per scegliere un vertice da colorare */
	int randomNumber;

	/* vertice scelto */
	int selectedVertex;


	for(i=0; i<vertexNumber; i++) greedyColoring[i]=INVALID;  //invalido il coloring

	/* inizializzo l'insieme dei vertici da colorare */
	unexaminedVertexSet=(int*)calloc(vertexNumber, sizeof(int));
	for(i=0; i<vertexNumber; i++) unexaminedVertexSet[i]=i;  //li inserisco tutti
	unexaminedNumber=vertexNumber;

	while(unexaminedNumber > 0){ //finche' ogni vertice e' stato esaminato

		randomNumber=getRandomNumber(unexaminedNumber);
				//ritorna un numero random nell'intervallo [0,unexaminedNumber[

		selectedVertex=unexaminedVertexSet[randomNumber];

		greedyColoring[selectedVertex]=1;    //si inizia col colore di indice minimo
		do{
			if(greedyColoring[selectedVertex] > KColorNumber){
				greedyColoring[selectedVertex]=0;   //non puo' essere colorato con K colori
				break;                              //passo al vertice successivo
			}
			assigned=true;
			for(j=pointerVector[selectedVertex]; j<pointerVector[selectedVertex+1]; j++){
				if(greedyColoring[selectedVertex]==greedyColoring[adjacentVector[j]]){
					assigned=false;                       //colore in conflitto					
					greedyColoring[selectedVertex]++;     //passo al colore successivo
					break;                   //esco dal for per riesaminare gli adiacenti
				}
			}//for
		}while(assigned==false);

		/* elimino il vertice esaminato dall'insieme */
		unexaminedNumber--;

		for(i=randomNumber; i<unexaminedNumber; i++)  //sposto i rimanenti di una posizione
			unexaminedVertexSet[i]=unexaminedVertexSet[i+1];

	}//while


	free(unexaminedVertexSet);
}//randomSequentialColoring




/***********************************************************************
 *                                                                     *
 * "Greedy saturation algorithm" utilizzato in "HEA".                  *
 * Ad ogni passo cerca di colorare il vertice con il minor numero di   *
 * classi di colore ammesse (cioe' quelle che non contengono nessun    *
 * vertice ad esso adiacente). Il colore utilizzato e' quello di       *
 * indice minimo.                                                      *
 * "greedyColoring" e' parametro di ingresso-uscita.                   *
 *                                                                     *
 ***********************************************************************/
void saturationGreedy(int* greedyColoring){

	/* indici generici */
	int  i, j, from, to;

	/* vettore contenente i vertici non ancora esaminati */
	int* unexaminedVertexSet;

	/* numero di vertici non ancora esaminati */
	int unexaminedNumber;

	/* vertice esaminato correntemente */
	int currentVertex;

	/* vertice scelto ad ogni passo dell'algoritmo */
	int selectedVertex;

	/* posizione del vertice scelto nel vettore dei vertici non esaminati */
	int selectedVertexPosition;

	/* minimo numero di classi di colore ammesse, per scegliere un vertice
	   (massimo grado di saturazione) */
	int minClassesNumber;

	/* vettore che indica quali classi di colore sono ammesse
	   per il vertice esaminato correntemente */
	bool* allowedClasses;

	/* numero di classi ammesse per il vertice considerato */
	int allowedClassesNumber;

	/* colore da assegnare al vertice scelto: quello la cui classe
	   ha indice minimo tra quelle ammesse */
	int selectedVertexColor;



	for(i=0; i<vertexNumber; i++) greedyColoring[i]=INVALID;  //invalido il coloring

	unexaminedVertexSet=(int*)calloc(vertexNumber-1, sizeof(int));
						//al massimo conterra' tutti i vertici, tranne il primo,
						//che viene colorato direttamente 

	allowedClasses=(bool*)calloc(KColorNumber+1, sizeof(bool));
						//KColorNumber+1 per non considerare la convenzione del C:
						//si ignora la posizione 0 dell'array e i colori vanno da 1 a K

	/* durante la prima iterazione dell'algoritmo e' inutile calcolare le
	   classi di colore ammesse: e' sufficiente assegnare al primo vertice
	   il colore 1 */
	greedyColoring[0]=1;

	/* inserisco i vertici rimanenti nell'insieme dei vertici da esaminare */
	for(i=1; i<vertexNumber; i++) unexaminedVertexSet[i-1]=i;
	unexaminedNumber=vertexNumber-1;


	while(unexaminedNumber > 0){  //si devono esaminare tutti i vertici

		/* scelta del vertice col minor numero di classi ammesse */

		minClassesNumber=INT_MAX;  //inizializzo a infinito positivo

		for(i=0; i<unexaminedNumber; i++){  //cerco tra i vertici non esaminati

			currentVertex=unexaminedVertexSet[i];  //vertice esaminato
			
			/* tutte le classi di colore sono inizialmente ammesse */
			for(j=1; j<=KColorNumber; j++) allowedClasses[j]=true;

			from=pointerVector[currentVertex];  //per localizzare gli adiacenti
			to=pointerVector[currentVertex+1];

			/* eliminazione delle classi di colore che non possono essere assegnate */
			for(j=from; j<to; j++)
				if(greedyColoring[adjacentVector[j]] > 0)
					allowedClasses[greedyColoring[adjacentVector[j]]]=false;

			/* calcolo del numero di classi ammesse dal vertice in esame */
			allowedClassesNumber=0;
			for(j=1; j<=KColorNumber; j++)
				if(allowedClasses[j]== true) allowedClassesNumber++;
			
			/* aggiorno il minimo numero di classi ammesse: il vertice selezionato
			   e' sempre il primo che aggiorna tale numero */
			if(allowedClassesNumber < minClassesNumber){
				minClassesNumber=allowedClassesNumber;
				selectedVertex=currentVertex;
				selectedVertexPosition=i;  //posizione nel vettore dei vertici non esaminati

				/* determino il colore di indice minimo */
				selectedVertexColor=0;
				for(j=1; j<=KColorNumber; j++)
					if(allowedClasses[j]==true){
						selectedVertexColor=j;
						break;
					} // se non viene trovato, selectedVertexColor rimane a 0
			}//if

		}//for
		
		/* assegno il colore al vertice */
		greedyColoring[selectedVertex]=selectedVertexColor;

		/* elimino il vertice esaminato dall'insieme */
		unexaminedNumber--;
		for(i=selectedVertexPosition; i<unexaminedNumber; i++)
			unexaminedVertexSet[i]=unexaminedVertexSet[i+1];
						//sposto i rimanenti di una posizione

	}//while

	free(unexaminedVertexSet);
	free(allowedClasses);

}//saturationGreedy



/***********************************************************************
 *                                                                     *
 * Algoritmo greedy basato su "saturationGreedy()" che sceglie i       *
 * vertici da colorare in modo casuale tra quelli che hanno lo stesso  *
 * numero (minimo) di classi di colore ammesse.                        *
 * "greedyColoring" e' parametro di ingresso-uscita.                   *
 *                                                                     *
 ***********************************************************************/
void randomSaturationGreedy(int* greedyColoring){

	/* indici generici */
	int  i, j, from, to;

	/* vettore contenente i vertici non ancora esaminati */
	int* unexaminedVertexSet;

	/* numero di vertici non ancora esaminati */
	int unexaminedNumber;

	/* vertice esaminato correntemente */
	int currentVertex;

	/* vertice scelto ad ogni passo dell'algoritmo */
	int selectedVertex;

	/* posizione del vertice scelto nel vettore dei vertici non esaminati */
	int selectedVertexPosition;

	/* minimo numero di classi di colore ammesse, per scegliere un vertice
	   (massimo grado di saturazione) */
	int minClassesNumber;

	/* vettore che indica quali classi di colore sono ammesse
	   per il vertice esaminato correntemente */
	bool* allowedClasses;

	/* numero di classi ammesse per il vertice considerato */
	int allowedClassesNumber;

	/* colore da assegnare al vertice scelto: quello la cui classe
	   ha indice minimo tra quelle ammesse */
	int selectedVertexColor;

	/* insieme di vertici che hanno lo stesso numero di classi ammesse: il
	   vertice da colorare deve essere scelto in modo random tra questi */
	int* minClassesVertexSet;

	/* numero di vertici che hanno lo stesso numero di classi ammesse */
	int minClassesVertexSetNumber;

	/* numero random per scegliere il vertice da colorare */
	int randomNumber;


	for(i=0; i<vertexNumber; i++) greedyColoring[i]=INVALID;  //invalido il coloring

	unexaminedVertexSet=(int*)calloc(vertexNumber-1, sizeof(int));
						//al massimo conterra' tutti i vertici, tranne uno, scelto
						//in modo casuale, che viene colorato direttamente 

	allowedClasses=(bool*)calloc(KColorNumber+1, sizeof(bool));
						//KColorNumber+1 per non considerare la convenzione del C:
						//si ignora la posizione 0 dell'array e i colori vanno da 1 a K

	minClassesVertexSet=(int*)calloc(vertexNumber, sizeof(int));


	/* durante la prima iterazione dell'algoritmo e' inutile calcolare le
	   classi di colore ammesse: e' sufficiente assegnare ad un primo vertice
	   random il colore 1 */
	randomNumber=getRandomNumber(vertexNumber);
	greedyColoring[randomNumber]=1;
	
	/* inserisco i vertici rimanenti nell'insieme dei vertici da esaminare */
	for(i=0; i<vertexNumber; i++){
		if(i<randomNumber) unexaminedVertexSet[i]=i;
		if(i>randomNumber) unexaminedVertexSet[i-1]=i;
	}
	unexaminedNumber=vertexNumber-1;


	while(unexaminedNumber > 0){  //si devono esaminare tutti i vertici
		/* scelta del vertice col minor numero di classi ammesse */

		minClassesNumber=INT_MAX;  //inizializzo a infinito positivo
		
		minClassesVertexSetNumber=0;  //insieme vuoto dei vertici da scegliere casualmente

		for(i=0; i<unexaminedNumber; i++){  //cerco tra i vertici non esaminati

			currentVertex=unexaminedVertexSet[i];  //vertice esaminato
			
			/* tutte le classi di colore sono inizialmente ammesse */
			for(j=1; j<=KColorNumber; j++) allowedClasses[j]=true;

			from=pointerVector[currentVertex];  //per localizzare gli adiacenti
			to=pointerVector[currentVertex+1];

			/* eliminazione delle classi di colore che non possono essere assegnate */
			for(j=from; j<to; j++)
				if(greedyColoring[adjacentVector[j]] > 0)
					allowedClasses[greedyColoring[adjacentVector[j]]]=false;

			/* calcolo del numero di classi ammesse dal vertice in esame */
			allowedClassesNumber=0;
			for(j=1; j<=KColorNumber; j++)
				if(allowedClasses[j]==true) allowedClassesNumber++;
			
			/* aggiorno il minimo numero di classi ammesse e memorizzo i vertici
			   con lo stesso numero (minimo) di tali classi */
			if(allowedClassesNumber <= minClassesNumber){

				/* resetto il vettore dei vertici con numero minimo di classi, nel caso
				   in cui si abbia un nuovo minimo */
				if(allowedClassesNumber < minClassesNumber){
					minClassesVertexSetNumber=0;
					minClassesNumber=allowedClassesNumber;  //nuovo minimo
				}

				minClassesVertexSet[minClassesVertexSetNumber]=currentVertex;
				minClassesVertexSetNumber++;

			}//if

		}//for
		
		/* scelgo un vertice in modo random tra quelli con minimo numero di classi
		   ammesse */
		randomNumber=getRandomNumber(minClassesVertexSetNumber);
		selectedVertex=minClassesVertexSet[randomNumber];

		/* determino il colore di indice minimo */
		for(j=1; j<=KColorNumber; j++) allowedClasses[j]=true;
		from=pointerVector[selectedVertex];  //per localizzare gli adiacenti
		to=pointerVector[selectedVertex+1];
		for(j=from; j<to; j++)				//elimino classi in conflitto
			if(greedyColoring[adjacentVector[j]] > 0)
				allowedClasses[greedyColoring[adjacentVector[j]]]=false;

		selectedVertexColor=0;
		for(j=1; j<=KColorNumber; j++)
			if(allowedClasses[j]==true){
				selectedVertexColor=j;
				break;
			} // se non viene trovato, selectedVertexColor rimane a 0
					
		/* assegno il colore al vertice */
		greedyColoring[selectedVertex]=selectedVertexColor;

		/* elimino il vertice esaminato dall'insieme */
		for(i=0; i<unexaminedNumber; i++)
			if(unexaminedVertexSet[i]==selectedVertex) selectedVertexPosition=i;
			//posizione nel vettore dei vertici non esaminati

		unexaminedNumber--;		

		for(i=selectedVertexPosition; i<unexaminedNumber; i++)
			unexaminedVertexSet[i]=unexaminedVertexSet[i+1];
						//sposto i rimanenti di una posizione

	}//while

	free(unexaminedVertexSet);
	free(allowedClasses);
	free(minClassesVertexSet);

}//randomSaturationGreedy
