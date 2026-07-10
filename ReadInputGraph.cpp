/*
 * Lettura dei dati del grafo da un file di input e costruzione di una
 * struttura dati formata da un vettore di puntatori ai vertici e da un
 * vettore di adiacenti.
 * Considerato il formato di rappresentazione dei grafi, per ottimizzare
 * tale fase si costruisce prima una matrice di adiacenza e poi si
 * inizializzano i due vettori.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CommonData.h"
#include "Utility.h"
#include "ReadInputGraph.h"



/* directory contenente le istanze */
//#define PATH_ISTANZE "Istanze"
#define PATH_ISTANZE ""


//#define PATH_SEPARATOR "\\"
#define PATH_SEPARATOR ""

/* per operare nella matrice di adiacenza, si utilizza la seguente macro
   che restituisce l'indice richiesto */
#define index(row, column) ( (row-1)*vertexNumber + (column-1) )
#define index_0(row, column) ( (row)*vertexNumber + (column) )			/* pss */

/* dichiarazione della procedura per l'apertura del file di input */
FILE* openFile(char* fileName);



/***********************************************************************
 *                                                                     *
 * Inizializza le strutture dati del grafo.                            *
 * "fileName" e' il nome del file dell'istanza.                        *
 *                                                                     *
 ***********************************************************************/
void readInputGraph(string fileName){
	/* file di input */
	FILE* inputFile;

	/* matrice di adiacenza implementata come un unico vettore */
	int* adjacencyMatrix;

	/* dimensione della matrice di adiacenza */
	int matrixDimension;

	/* indici generici */
	int i, j, v1, v2;

	/* indice per la costruzione del vettore degli adiacenti */
	int indexV;

	/* controllo che ogni vertice abbia un adiacente */
	bool check;


	/* apertura del file dell'istanza */
	inputFile=openFile((char*)fileName.c_str());

	/* lettura delle dimensioni del grafo */
	const int BUFMAX = 250;
	char line[BUFMAX + 1];
	while (fgets(line, BUFMAX, inputFile)){
		if (line[0] != 'p') continue;
		else break;
		puts(line);
	}

	//ignora "p edge" oppure "e edge"
	sscanf(line, "%*s %*s %d %d", &vertexNumber, &edgeNumber);
	
	//ORIGINAL CODE- primitive
	//fscanf(inputFile, "%*s %*s %d %d", &vertexNumber, &edgeNumber);
	//							//ignora "p edge" oppure "e edge"

	if( (vertexNumber < 2) || (edgeNumber < 1) ){
		puts("readInputGraph: Errore nelle dimensioni del problema.");
		printf("%d, %d \n", vertexNumber, edgeNumber);
		exit(3);
	}

#ifdef print_original_logs_vcph
	printf("\nIstanza: %s\n\n", fileName);
	printf("Numero vertici = %d\nNumero lati = %d\n\n", vertexNumber, edgeNumber);
#endif

	all_info_VCPh.N = vertexNumber;
	all_info_VCPh.M = edgeNumber;


	/* creazione della matrice di adiacenza di dimensione (vertexNumber^2);
	   la matrice e' costituita da un vettore unico al quale si accede
	   secondo una logica a puntatori */
	matrixDimension=vertexNumber*vertexNumber;
	adjacencyMatrix=(int*)calloc(matrixDimension, sizeof(int));
	for(i=0; i<matrixDimension; i++) adjacencyMatrix[i]=false;

	/* lettura dei lati */
	for(i=0; i<edgeNumber; i++){
		if(fscanf(inputFile, "%*s %d %d", &v1, &v2) !=2){
										//ignora "e" all'inizio di ogni riga
			puts("readInputGraph: Errore nei dati in input durante la lettura dei lati.");
			exit(4);
		}

		if( v1==v2 || v1<1 || v1>vertexNumber || v2<1 || v2>vertexNumber ){
			puts("readInputGraph: Errore nei dati in input: incoerenza nei vertici.");
			exit(5);
		}
		adjacencyMatrix[index(v1,v2)]=true;  //uso la macro per trovare l'indice
		adjacencyMatrix[index(v2,v1)]=true;
	}//for

	/* controllo che il file sia terminato, consumando i caratteri bianchi */
	if(fscanf(inputFile, "%*s") != EOF){
		puts("readInputGraph: Errore al termine della lettura dell'input: il file contiene ulteriori dati.");
		exit(6);
	}
	fclose(inputFile);

	/* inizializzazione delle strutture dati che memorizzano il grafo */
	pointerVector=(int*)calloc(vertexNumber+1, sizeof(int));
	adjacentVector=(int*)calloc(edgeNumber*2, sizeof(int));
										//il doppio del numero dei lati
	adjacentNumberVector=(int*)calloc(vertexNumber, sizeof(int));


	pointerVector[0]=0; //il primo punta all'inizio del vettore degli adiacenti
	indexV=0;           //mantiene la posizione nel vettore dei vertici adiacenti

	check=false;  //controllo che il grafo non abbia vertici senza alcun adiacente

	for(i=0; i<vertexNumber; i++){                         //riga nella matrice
		for(j=0; j<vertexNumber; j++){                     //colonna
			if(adjacencyMatrix[i*vertexNumber+j]==true){
				adjacentVector[indexV]=j;
				indexV++;
				check=true;
			}
		}//for
		if(check==false){
			printf("readInputGraph: Il vertice %d non ha alcun adiacente.\n\n", i+1);
		}
		pointerVector[i+1]=indexV;
		check=false;
	}//for

	/* inizializzazione del vettore contenente il numero di adiacenti per ogni vertice */
	for (i=0;i<vertexNumber;i++)
		adjacentNumberVector[i]=pointerVector[i+1]-pointerVector[i];

	free(adjacencyMatrix);
}



/***********************************************************************
 *                                                                     *
 * Initialization from ugraph type 								       *
 * (pss-04/11/2021)  												   *
 *                                                                     *
 ***********************************************************************/
void readInputGraph(ugraph & ug)
{
	//TODO-ADD CODE HERE
	

	/* matrice di adiacenza implementata come un unico vettore */
	int* adjacencyMatrix;

	/* dimensione della matrice di adiacenza */
	int matrixDimension;

	/* indici generici */
	int i, j, v1, v2;

	/* indice per la costruzione del vettore degli adiacenti */
	int indexV;

	/* controllo che ogni vertice abbia un adiacente */
	bool check;


	vertexNumber = ug.number_of_vertices();
	edgeNumber = ug.number_of_edges();

	all_info_VCPh.N = vertexNumber;
	all_info_VCPh.M = edgeNumber;

	

#ifdef print_original_logs_vcph
	ug.print_data();
	printf("\nIstanza: %s\n\n", ug.get_name().c_str());
	printf("Numero vertici = %d\nNumero lati = %d\n\n", vertexNumber, edgeNumber);
#endif


	/* creazione della matrice di adiacenza di dimensione (vertexNumber^2);
	   la matrice e' costituita da un vettore unico al quale si accede
	   secondo una logica a puntatori */
	matrixDimension = vertexNumber * vertexNumber;
	adjacencyMatrix = (int*)calloc(matrixDimension, sizeof(int));
	for (i = 0; i < matrixDimension; i++) adjacencyMatrix[i] = 0;

	for (int v = 0; v < vertexNumber-1 ; v++) {
		for (int w = v + 1; w < vertexNumber; w++) {
			if (ug.is_edge(v, w)) {
				adjacencyMatrix[index_0(v, w)] = 1;		//uso la macro per trovare l'indice
				adjacencyMatrix[index_0(w, v)] = 1;
			}
		}
	}
	///* lettura dei lati */
	//for (i = 0; i < edgeNumber; i++) {
	//	if (fscanf(inputFile, "%*s %d %d", &v1, &v2) != 2) {
	//		//ignora "e" all'inizio di ogni riga
	//		puts("readInputGraph: Errore nei dati in input durante la lettura dei lati.");
	//		exit(4);
	//	}

	//	if (v1 == v2 || v1<1 || v1>vertexNumber || v2<1 || v2>vertexNumber) {
	//		puts("readInputGraph: Errore nei dati in input: incoerenza nei vertici.");
	//		exit(5);
	//	}
	//	adjacencyMatrix[index(v1, v2)] = true;  //uso la macro per trovare l'indice
	//	adjacencyMatrix[index(v2, v1)] = true;
	//}//for
	   	 	

	/* inizializzazione delle strutture dati che memorizzano il grafo */
	pointerVector = (int*)calloc(vertexNumber + 1, sizeof(int));
	adjacentVector = (int*)calloc(edgeNumber * 2, sizeof(int));
	//il doppio del numero dei lati
	adjacentNumberVector = (int*)calloc(vertexNumber, sizeof(int));


	pointerVector[0] = 0; //il primo punta all'inizio del vettore degli adiacenti
	indexV = 0;           //mantiene la posizione nel vettore dei vertici adiacenti

	check = false;  //controllo che il grafo non abbia vertici senza alcun adiacente

	for (i = 0; i < vertexNumber; i++) {                         //riga nella matrice
		for (j = 0; j < vertexNumber; j++) {                     //colonna
			if (adjacencyMatrix[i*vertexNumber + j] == true) {
				adjacentVector[indexV] = j;
				indexV++;
				check = true;
			}
		}//for
		if (check == false) {
			printf("readInputGraph: Il vertice %d non ha alcun adiacente.\n\n", i + 1);
		}
		pointerVector[i + 1] = indexV;
		check = false;
	}//for

	/* inizializzazione del vettore contenente il numero di adiacenti per ogni vertice */
	for (i = 0; i < vertexNumber; i++)
		adjacentNumberVector[i] = pointerVector[i + 1] - pointerVector[i];

	free(adjacencyMatrix);

}
//readInputGraph




/***********************************************************************
 *                                                                     *
 * Procedura che apre in lettura il file dell'istanza.                 *
 *                                                                     *
 ***********************************************************************/
FILE* openFile(char* fileName){

	/* percorso nel file system */
	char* pathInputFile;

	/* file di input */
	FILE* inputFile;

	/* costruisco il path del file dell'istanza */
	pathInputFile=(char*)calloc( strlen(PATH_ISTANZE) + strlen(PATH_SEPARATOR) +
							     strlen(fileName) + 1, sizeof(char) );
	if(pathInputFile==NULL){
		puts("openFile: out of memory");
		exit(1);
	}

	pathInputFile[0]=NULL;
	strcat(pathInputFile, PATH_ISTANZE);
	strcat(pathInputFile, PATH_SEPARATOR);
	strcat(pathInputFile, fileName);

	/* apertura del file in sola lettura */
	if( (inputFile=fopen(pathInputFile, "r")) == NULL ){
		printf("File non esistente: %s.\n", pathInputFile);
		exit(2);
	}

	free(pathInputFile);

	return inputFile;
}//openFile