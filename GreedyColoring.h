/*
 * Dichiarazione di algoritmi greedy per il coloring.
 */


#ifndef GREEDY_COLORING_HEADER
#define GREEDY_COLORING_HEADER



/***********************************************************************
 *                                                                     *
 * Greedy sequenziale che assegna ad ogni vertice il colore            *
 * ammissibile di indice minimo. Le classi di colore sono al piu' K,   *
 * pertanto un vertice che non puo' essere colorato ha colore 0.       *
 * "greedyColoring" e' parametro di ingresso-uscita.                   *
 *                                                                     *
 ***********************************************************************/
void sequentialColoring(int* greedyColoring);



/***********************************************************************
 *                                                                     *
 * Greedy sequenziale simile a "sequentialColoring()" ma che permette  *
 * di indicare il vertice da cui iniziare la colorazione.              *
 * "greedyColoring" e' parametro di ingresso-uscita.                   *
 *                                                                     *
 ***********************************************************************/
void sequentialColoringFromVertex(int fromVertex, int* greedyColoring);



/***********************************************************************
 *                                                                     *
 * Algoritmo greedy di colorazione che permette di ottenere            *
 * colorazioni differenti mediante una scelta random della sequenza    *
 * dei vertici da colorare. E' basato su "sequentialColoring()".       *
 * "greedyColoring" e' parametro di ingresso-uscita.                   *
 *                                                                     *
 ***********************************************************************/
void randomSequentialColoring(int* greedyColoring);



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
void saturationGreedy(int* greedyColoring);



/***********************************************************************
 *                                                                     *
 * Algoritmo greedy basato su "saturationGreedy()" che sceglie i       *
 * vertici da colorare in modo casuale tra quelli che hanno lo stesso  *
 * numero (minimo) di classi di colore ammesse.                        *
 * "greedyColoring" e' parametro di ingresso-uscita.                   *
 *                                                                     *
 ***********************************************************************/
void randomSaturationGreedy(int* greedyColoring);



#endif
