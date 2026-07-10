/*
 * Operatore di crossover "GPX" ("Greedy Partition Crossover"),
 * definito in "HEA", utilizzato per produrre una nuova
 * configurazione di coloring ("offspring") derivante da due "parents".
 */

#include <stdlib.h>
#include <limits.h>		//per INT_MAX
#include "CommonData.h"
#include "Utility.h"

//per ignorare i vertici ai quali e' gia' stata assegnata una classe di colore
#define SKIP -1

//per indicare i vertici del figlio non ancora presi in considerazione
#define UNSET -1

//determina la classe di colore con il maggior numero di vertici
int getColorClassMax(int* parent);

//procedura utilizzata al termine del crossover per colorare i vertici del
//figlio che sono rimasti non assegnati (basata su "saturationGreedy()")
void saturationGreedyAfterCrossover(int* offspring);

//procedura utilizzata al termine del crossover per colorare i vertici del
//figlio che sono rimasti non assegnati (basata su "sequentialColoring()")
void sequentialColoringAfterCrossover(int* offspring);



/* PSS-copia di lavoro dei genitori */
int* s1 = NULL;
int* s2 = NULL;
int* colorClass = NULL;



/***********************************************************************
 *                                                                     *
 * "parentS1" e "parentS2" sono le configurazioni di colore dei        *
 * genitori.                                                           *
 * Il vettore "offspring" conterra' il figlio creato.                  *
 *                                                                     *
 ***********************************************************************/
void crossover(int* parentS1, int* parentS2, int* offspring){

	/* indici generici */
	int i;

	/* classe di colore da assegnare ai vertici da colorare del figlio */
	int color;

	/* copia di lavoro dei genitori */
	//static int* s1=NULL;
	//static int* s2=NULL;

	/* genitore selezionato di cui determinare la classe di colore piu' ampia */
	int* currentParent;

	/* genitore non selezionato, dal quale e' necessario rimuovere i vertici
	   gia' assegnati */
	int* otherParent;

	/* classe di colore di cardinalita' massima */
	int colorClassMax;

	/* indica la presenza di ulteriori vertici colorati nei genitori
	   in modo da terminare il crossover nel caso in cui non sia piu' possibile
	   selezionare una classe di colore dai genitori */
	bool availableS1;
	bool availableS2;


	/* inizializzazione del figlio: nessuno dei suoi vertici e' stato esaminato */
	for(i=0; i<vertexNumber; i++) offspring[i]=UNSET;

	/* effettuo la copia di lavoro dei genitori */
	//if(s1==NULL)
	//	s1=(int*)calloc(vertexNumber, sizeof(int));
	//if(s2==NULL)
	//	s2=(int*)calloc(vertexNumber, sizeof(int));
	for(i=0; i<vertexNumber; i++){
		s1[i]=parentS1[i];
		s2[i]=parentS2[i];
	}

	color=1; //i primi vertici individuati nei genitori verranno colorati nel
	         //figlio con classe 1

	availableS1=true;
	availableS2=true;

	/* differentemente dal crossover presentato nell'articolo "HEA",
	   il presente crossover deve tener conto dei vertici scolorati di
	   entrambi i genitori, con la possibilita', quindi, che non si
	   possano ottenere piu' vertici dai genitori da assegnare al figlio */
	
	/* itero finche' sono disponibili classi di colore nel figlio e finche'
	   sono disponibili vertici nei genitori */
	while( (color <= KColorNumber) && (availableS1 || availableS2) ){

		/* considero un genitore alla volta, alternativamente (nel caso in entrambi
           siano disponibili dei vertici) */
		if(availableS1 && availableS2){
			if(color%2 == 1){      //iterazione dispari
				currentParent=s1;
				otherParent=s2;
			}else{                //iterazione pari
				currentParent=s2;
				otherParent=s1;
			}
		}else{ //caso in cui sia rimasto da utilizzare un solo genitore con vertici disponibili
			if(availableS1){
				currentParent=s1;
				otherParent=s2;
			}else{
				currentParent=s2;
				otherParent=s1;
			}
		}

		/* determinazione della classe di colore di cardinalita' massima nel genitore */
		colorClassMax=getColorClassMax(currentParent);

		/* caso in cui non ci siano piu' vertici da assegnare nel genitore corrente */
		if(colorClassMax == 0){
			if(currentParent==s1) availableS1=false;
			else availableS2=false;

			continue;  //continuo il ciclo senza incrementare "color"
		}

		for(i=0; i<vertexNumber; i++){
			if(currentParent[i]==colorClassMax){
				
				/* coloro il vertice nel figlio con il colore di indice
				   minore disponibile */
				offspring[i]=color;
				
				/* elimino il vertice da entrambi i genitori */
				currentParent[i]=SKIP;
				otherParent[i]=SKIP;
			}
		}//for

		/* passo al colore successivo da assegnare ai vertici del figlio */
		color++;

	}//while

	/* tentativo di colorazione dei vertici del figlio rimasti non assegnati
	   gli algoritmi greedy a disposizione sono:
	     - greedy di tipo sequenziale
	     - greedy di tipo saturation	   
	   si sceglie di utilizzare il sequenziale per ridurre il costo computazionale */

	sequentialColoringAfterCrossover(offspring);

	//free PSS - TESTING
	/*free(s1);
	free(s2);
	s1 = NULL;
	s2 = NULL;*/

}//crossover



/***********************************************************************
 *                                                                     *
 * Procedura utilizzata in "crossover()" che determina, nel genitore,  *
 * la classe di colore con il maggior numero di vertici.               *
 * Ritorna "0" nel caso in cui non siano piu' disponibili classi di    *
 * colore dal genitore corrente, cioe' nel caso in cui i vertici       *
 * rimanenti siano tutti scolorati oppure siano gia' stati tutti       *
 * assegnati.                                                          *
 *                                                                     *
 ***********************************************************************/
int getColorClassMax(int* parent){

	/* indici generici */
	int i, max;

	/* vettore di colori che indica il numero di vertici contenuto in
	   ogni classe */
	//static int* colorClass=NULL;

	/* valore di ritorno: classe di cardinalita' massima */
	int colorClassMax;


	/*if(colorClass==NULL)
		colorClass=(int*)calloc(KColorNumber+1, sizeof(int));*/

	/* inizializzazione: tutte le classi non contengono nessun vertice */
	for(i=1; i<=KColorNumber; i++) colorClass[i]=0;

	/* calcolo il numero di vertici contenuti in ogni classe di colore,
	   ignorando i vertici gia' assegnati e quelli non colorati */
	for(i=0; i<vertexNumber; i++)
		if( (parent[i] != SKIP) && (parent[i] != 0) ) colorClass[parent[i]]++;

	max=0;           //massima cardinalita' trovata
	colorClassMax=0; //valore ritornato nel caso in cui non sia disponibile nessuna classe

	for(i=1; i<=KColorNumber; i++)
		if(colorClass[i] > max){   //sceglie la prima classe incontrata di massima
			colorClassMax=i;	   //cardinalita', ignorando le altre
			max=colorClass[i];
		}

	//PSS- JUST TESTING TRYING TO FIX MEM ALLOC- TODO@not here
	/*free(colorClass);
	colorClass = NULL;*/

	return colorClassMax;
}



/***********************************************************************
 *                                                                     *
 * Algoritmo greedy basato su "sequentialColoring()" che viene         *
 * utilizzato dal "crossover" per colorare i vertici del figlio che    *
 * sono rimasti non assegnati.                                         *
 * "offspring" e' il figlio di cui si deve cercare di colorare i       *
 * vertici rimasti non assegnati.                                      *
 *                                                                     *
 ***********************************************************************/
void sequentialColoringAfterCrossover(int* offspring){
	
	/* indici generici */
	int i, j;
	
	/* indica se e' stato assegnato un colore al vertice in esame */
	bool assigned;
	
	for(i=0; i<vertexNumber; i++){
		if(offspring[i] == UNSET){

			offspring[i]=1;    //si inizia col colore di indice minimo
			do{
				if(offspring[i] > KColorNumber){
					offspring[i]=0;              //non puo' essere colorato con K colori
					break;                       //passo al vertice successivo
				}
				assigned=true;
				for(j=pointerVector[i]; j<pointerVector[i+1]; j++){
					if(offspring[i]==offspring[adjacentVector[j]]){   //colore in conflitto
						assigned=false;
						offspring[i]++;     //passo al colore successivo
						break;              //esco dal for per riesaminare gli adiacenti
					}
				}//for
			}while(assigned==false);

		}//if
	}//for

}//sequentialColoringAfterCrossover



/***********************************************************************
 *                                                                     *
 * Algoritmo greedy basato su "saturationGreedy()" che viene           *
 * utilizzato dal "crossover" per colorare i vertici del figlio che    *
 * sono rimasti non assegnati.                                         *
 * "offspring" e' il figlio di cui si deve cercare di colorare i       *
 * vertici rimasti non assegnati.                                      *
 *                                                                     *
 ***********************************************************************/
void saturationGreedyAfterCrossover(int* offspring){

	/* indici generici */
	int  i, j, from, to;

	/* vettore contenente i vertici non ancora esaminati */
	static int* unexaminedVertexSet=NULL;

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
	static bool* allowedClasses=NULL;

	/* numero di classi ammesse per il vertice considerato */
	int allowedClassesNumber;

	/* colore da assegnare al vertice scelto: quello la cui classe
	   ha indice minimo tra quelle ammesse */
	int selectedVertexColor;


	if(unexaminedVertexSet==NULL)
		unexaminedVertexSet=(int*)calloc(vertexNumber, sizeof(int));
						//al massimo conterra' tutti i vertici

	if(allowedClasses==NULL)
		allowedClasses=(bool*)calloc(KColorNumber+1, sizeof(bool));
						//KColorNumber+1 per non considerare la convenzione del C:
						//si ignora la posizione 0 dell'array e i colori vanno da 1 a K

	/* inserisco i vertici non assegnati (che hanno valore UNSET) nell'insieme
	   dei vertici da esaminare */
	unexaminedNumber=0;
	for(i=0; i<vertexNumber; i++)
		if(offspring[i]==UNSET){
			unexaminedVertexSet[unexaminedNumber]=i;
			unexaminedNumber++;
		}


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
				if(offspring[adjacentVector[j]] > 0)
					allowedClasses[offspring[adjacentVector[j]]]=false;

			/* calcolo del numero di classi ammesse dal vertice in esame */
			allowedClassesNumber=0;
			for(j=1; j<=KColorNumber; j++)
				if(allowedClasses[j]==true) allowedClassesNumber++;
			
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
		offspring[selectedVertex]=selectedVertexColor;

		/* elimino il vertice esaminato dall'insieme */
		unexaminedNumber--;
		for(i=selectedVertexPosition; i<unexaminedNumber; i++)
			unexaminedVertexSet[i]=unexaminedVertexSet[i+1];
						//sposto i rimanenti di una posizione

	}//while

	//PSS TESTING
	free(unexaminedVertexSet);
	free(allowedClasses);
	unexaminedVertexSet = NULL;
	allowedClasses = NULL;


}//saturationGreedyAfterCrossover
