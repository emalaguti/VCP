/*
 * Dichiarazione delle strutture dati globali.
 */



#ifndef COMMON_DATA_HEADER
#define COMMON_DATA_HEADER

#define LOG_VCPH_FILENAME "info_VCPh.txt"
#define LOG_VCPH_DETAILED_RESULTS_FILENAME "info_VCPh_detail.txt"

//#define print_original_logs_vcph

#include <string>
#include <vector>

/* numero dei vertici del grafo */
extern int vertexNumber;

/* numero dei lati (non orientati) del grafo */
extern int edgeNumber;

/* vettore di puntatori che indicano la posizione logica
   all'interno del vettore degli adiacenti */
extern int* pointerVector;

/* vettore dei vertici adiacenti */
extern int* adjacentVector;

/* vettore contenente il numero di adiacenti per ogni vertice (grado)
   (serve per lo score sia nel Tabu Search sia nella gestione della popolazione) */
extern int* adjacentNumberVector;

/* numero di colori K */
extern int KColorNumber;

/* nome dell'istanza in esame */
extern std::string  instanceName;					//PSS-TODO@ check-allocation, it is wrong!

/*seme funzioni random*/
extern int set_seed;




/* pss-04/11/2021 */
struct info_VCPh {
	friend std::ostream& operator<<(std::ostream& o, const info_VCPh& k);

	enum stat_t {ERR=-1, OK=0};
	std::string name;
	int N;
	int M;
	int target_K;
	int ITER_TABU;
	int ALFA_TABU;
	int RAND_TABU;
	int CONST_TABU;
	int pop_size;
	int TIME_LIMIT;
	double elapsed_time;
	int random_seed;
	bool is_sol_found;
	std::vector<int> sol;
	stat_t status;

	std::string log_filename;
	std::string log_detailed_results_filename;

	info_VCPh():    log_filename(LOG_VCPH_FILENAME), log_detailed_results_filename(LOG_VCPH_DETAILED_RESULTS_FILENAME), target_K(-1), N(-1), M(-1), ITER_TABU(-1), ALFA_TABU(-1), RAND_TABU(-1), CONST_TABU(-1),
						pop_size(0), TIME_LIMIT(-1), random_seed(1000), is_sol_found(false), status(OK), elapsed_time(0.0) {}
	void reset() {
		name = std::string(); log_filename = LOG_VCPH_FILENAME; log_detailed_results_filename = LOG_VCPH_DETAILED_RESULTS_FILENAME; target_K = -1; N = -1; M = -1; ITER_TABU = -1; ALFA_TABU = -1; RAND_TABU = -1; CONST_TABU = -1;
		pop_size = 0; TIME_LIMIT = -1; random_seed = 1000; is_sol_found = false, status=OK, elapsed_time=0.0;
	}

	//I/O
	std:: ostream& print_params(std::ostream&);
	std::ostream& print_sol(std::ostream&);
	//std::ofstream& print_to_file(std::ofstream&);

	 /* apertura del file in scrittura */
//   fp = fopen ("risultati_gen.txt","a+");
   //fp = fopen(all_info_VCPh.log_detailed_results_filename.c_str(), "a+");
   ////tempo=printElapsedTime2();
   //tempo = getElapsedTime();
   //fprintf(fp,"instanceName = %s  KColorNumber = %d  random_seed = %d  time = %f\n", instanceName, KColorNumber,set_seed,tempo);
   //fclose (fp);

	
};

extern info_VCPh all_info_VCPh;


#endif
