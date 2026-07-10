/*
 * Main dell'applicazione — VERSIONE PARALLELIZZATA
 *
 * Uso:
 *   ./vcp instanceName K tabuIter alpha rand const popSize timeLimit seed [threads]
 *
 * Parametri posizionali:
 *   instanceName   percorso del file di istanza (formato DIMACS .col)
 *   K              numero target di colori (>= 1)
 *   tabuIter       iterazioni del Tabu Search per individuo (>= 1)
 *   alpha          parametro alpha per il calcolo del tabu tenure (>= 0)
 *   rand           parametro rand  per il calcolo del tabu tenure (>= 0)
 *   const          parametro const per il calcolo del tabu tenure (>= 0)
 *   popSize        dimensione della popolazione (>= 2)
 *   timeLimit      limite di tempo in secondi (>= 1)
 *   seed           seme per il generatore random (>= 0)
 *   threads        [opzionale] numero di thread OpenMP (default 0 = tutti i core)
 *                  1 = esecuzione sequenziale (utile per il debug)
 *
 * Esempi:
 *   ./vcp graph.col 10 10000 1.0 1 1 16 3600 1000
 *   ./vcp graph.col 10 10000 1.0 1 1 16 3600 1000 8
 *   ./vcp graph.col 10 10000 1.0 1 1 16 3600 1000 1
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "run_VCP_Manager.h"


int main(int argc, char** argv) {

    /* controllo argomenti: minimo 9 obbligatori, threads opzionale */
    if (argc < 10 || argc > 11) {
        printf(
            "Uso:\n"
            "  %s instanceName K tabuIter alpha rand const popSize timeLimit seed [threads]\n"
            "\n"
            "Parametri:\n"
            "  instanceName  file di istanza DIMACS (.col)\n"
            "  K             numero target di colori  (>= 1)\n"
            "  tabuIter      iterazioni Tabu Search   (>= 1)\n"
            "  alpha         tabu tenure alpha        (>= 0)\n"
            "  rand          tabu tenure rand         (>= 0)\n"
            "  const         tabu tenure const        (>= 0)\n"
            "  popSize       dimensione popolazione   (>= 2)\n"
            "  timeLimit     time limit in secondi    (>= 1)\n"
            "  seed          seme random              (>= 0)\n"
            "  threads       thread OpenMP [opz.]     (0 = tutti i core, default 0)\n"
            "\n"
            "Esempi:\n"
            "  %s graph.col 10 10000 1.0 1 1 16 3600 1000\n"
            "  %s graph.col 10 10000 1.0 1 1 16 3600 1000 8\n",
            argv[0], argv[0], argv[0]
        );
        return 1;
    }

    /* lettura parametri obbligatori */
    std::string instanceName  = argv[1];
    int         K             = atoi(argv[2]);
    int         tabuIter      = atoi(argv[3]);
    double      alpha         = atof(argv[4]);
    int         randParam     = atoi(argv[5]);
    int         constParam    = atoi(argv[6]);
    int         popSize       = atoi(argv[7]);
    int         timeLim       = atoi(argv[8]);
    int         seed          = atoi(argv[9]);

    /* parametro opzionale: numero di thread (default 0 = tutti i core) */
    int numThreads = (argc == 11) ? atoi(argv[10]) : 0;

    /* struttura di output */
    info_VCPh info;

    /* esecuzione */
    heurVCP::mmt_VCPheur(
        instanceName,
        K,
        info,
        tabuIter,
        alpha,
        randParam,
        constParam,
        popSize,
        timeLim,
        seed,
        LOG_VCPH_FILENAME,  /* file di log default */
        true,               /* verbose */
        numThreads
    );

    /* riepilogo finale */
    if (info.is_sol_found) {
        printf("\nSoluzione trovata:  K=%d  tempo=%.1fs  thread=%d\n",
               info.target_K, info.elapsed_time, info.num_threads);
    } else {
        printf("\nNessuna soluzione trovata in %.1fs con %d thread.\n",
               info.elapsed_time, info.num_threads);
    }

    return info.is_sol_found ? 0 : 2;
}
