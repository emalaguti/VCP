/*
 * Definizione delle strutture dati globali.
 */

#include "CommonData.h"			
#include <iostream>
#include <string>


using namespace std;

/* numero dei vertici del grafo */
int vertexNumber;

/* numero dei lati (non orientati) del grafo */
int edgeNumber;

/* vettore di puntatori che indicano la posizione logica
   all'interno del vettore degli adiacenti */
int* pointerVector = NULL;

/* vettore dei vertici adiacenti */
int* adjacentVector = NULL;

/* vettore contenente il numero di adiacenti per ogni vertice (grado)
   (serve per lo score sia nel Tabu Search sia nella gestione della popolazione) */
int* adjacentNumberVector = NULL;

/* numero di colori K */
int KColorNumber;

/* nome dell'istanza in esame-changed to string type */
string instanceName;											

/*seme funzioni random*/
int set_seed;

/* the one and only global info -pss */
info_VCPh all_info_VCPh;

std::ostream& operator<<(ostream& o, const info_VCPh& k) {
	o << k.name << "\t" << k.N << "\t" << k.M << "\t" << k.target_K <<  "\t" 
		<< k.is_sol_found <<"\t"
		<< k.TIME_LIMIT << "\t" << k.elapsed_time << "\t"  
		<< k.ALFA_TABU << "\t" << k.ITER_TABU << "\t" << k.CONST_TABU << "\t"
		<< k.random_seed << "\t"
		<< k.status
		<< std::endl;
	return o;
}

std::ostream& info_VCPh::print_params(std::ostream& o) {
	cout<<endl;
	cout<<"*****************************"<<endl;
	cout<<"DATA:" << name << "\t N:" << N << "\t M:" << M << "\t D:" << 2 * M / (float)((N - 1)*N) << endl;
	cout << "TARGET K:" << target_K << endl;
	cout<<  "TIME_LIMIT:" << TIME_LIMIT << endl;
	cout << "TABU_ITER:" << ITER_TABU << endl;
	cout << "TABU_ALFA:" << ALFA_TABU << endl;
	cout << "TABU_CONST:" << CONST_TABU << endl;
	cout << "POP_SIZE:" << pop_size << endl;
	cout <<	"*****************************" <<endl;
	return o;
}

std::ostream& info_VCPh::print_sol(std::ostream& o) {
	cout << endl;
	cout << "*****************************" << endl;
	cout << "DATA:" << name << "\t N:" << N << "\t M:" << M << endl;
	cout << "TARGET K:" << target_K << endl;
	cout << "TIME_ELAPSED:" << elapsed_time << endl;
	cout << "SOL FOUND:" << is_sol_found << endl;
	cout << "*****************************" <<endl;
	return o;
}






