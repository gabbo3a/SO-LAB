#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

#include "utility.h"

// Function to distribution
/* Nota: Questa funzione è stata inserita qui per comodita, potrebbe anche provedire da
//       un altro file. Ho deciso di non passarla come puntatore a get_atom_number poiche
//       non è previsto che cambi a runtime (se dovesse cambiare le considerei simulazioni
//       diverse).*/
/* Nota: Nonostante non sia un granche di funzione, poiche vengo sempre gli stessi numeri
//       nella stessa seq, non ho molte energie per studiare la generazioen di numeri random
//       di basso livello per ottenere una "randomicita" soddisfaciente :).*/
int distribution(int min, int max) {
    return get_lrand(min, max);
}
 
// Function to init N_ATOMI_INIT
void init_atoms(int n, pid_t error_receiver) {
    const int N_ATOM_MAX = atoi(getenv("N_ATOM_MAX"));   // Max limit (no static, colpa getenv)
    
    pid_t pid;
    for (int i = 0; i < n; i++) {
        // Nota: La generazione del numero atomi è stata inserita qui
        //       poiche da testo deve essere eseguita dal padre. Questo
        //       pero non massimizza la concorrenza poiche il padre
        //       dovra eseguire op; fork; ... . Sarebbe meglio che sia
        //       compito del atomo stesso poche è una sua "info privata".
        const char* atom_number = ntos(distribution(1, 100));    // Generate atom number

        /* Nota: Il passagio effetivo di atom_n avviene tramite la copia
        //       del processo ma con exec.. si perderebbe informazione per
        //       questo è necessario ripassarlo.*/
        /* Nota: Avere lo switch con dei numeri letterali è terribili quindi
        //       li ho sostituiti, pero è molto urtante che li ci sia il default.
        //       In un linguaggio con le closure avrei fatto una mappa/array con
        //       le procedure di gestione scrivendo un codice piu carino :<.
        //       Il mio mio gcc mi permette le closure pero si dice che non sia
        //       standard.*/
        pid = fork();
        switch (pid) {
            case FORK_ERROR: 
                sgerrorm(error_receiver, SIGTERM, MELTDOWN_TERMINATION);  
            case CHILD_PROCESS:
                execl("./build/exe/atomo", atom_number, NULL);
                gerrorm("Error executing execl");
            default: 
                free((void*) atom_number); // Dealloca la stringa solo nel padre
                break;
        }
    }
}