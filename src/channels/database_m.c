
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

#include "utility.h"
#include "database_m.h"
#include "semaphore_m.h"

// Explanation: Modulo di gestione del "database" (array/buffer "inlimitato" piu adatto) di record di atomi,
//              dato che molte operazzioni sono condivise da piu processi è stato implemenato. Incaspula anche
//              informazioni che ai chiamanti sarebbero inutili come fd o il nome del file.
// Nota:        Come implementazione non è delle migliori anzi alcume funzoini si sono dimostarte non utili
//              pero in ottica di modularita del progetto rende molto piu comodo utilizzare il file gestendo
//              errori ecc.

static const char* pathname = "atom_records.db";
static int fd               = -1;

// Function to open file
void init_db() {
    fd = open(pathname, 
        O_CREAT | O_RDWR, S_IRUSR | S_IWUSR
    );
    if (fd == -1)
        gerrorm("Opening/Creating file error");
}

// Function to check if already open with init_db
static void fopen_check() {
    if (fd == -1)
        gerrorm("DBM file dont open");
}

// Function to clear db file (thsafe)
void clear_db() {
    fopen_check();
    if (ftruncate(fd, 0) == GENERIC_ERROR)         
        gerrorm("Error cleaning file");
}

// Function to get length (number record)
size_t get_number_atom_records() {
    fopen_check();

    // Count number of records
    off_t nbyte = lseek(fd, 0, SEEK_END);
    if (nbyte == -1) 
        gerrorm("Error getting file size");

    return nbyte / sizeof(AtomRecord);
}

// Function to get index by pid
int get_atom_index(const pid_t pid) {
    size_t n_atom = get_number_atom_records();

    // Sequential search on records
    // Nota: L'idea iniziale era di fare una ricerca dicotomica dato che i pid sono unici
    //       (come le key di un db) e crescenti, ma poi pensadoci meglio penso di non poter
    //       farlo perche non ordinati. Se con il master che crea effetivamnte i primi
    //       N_ATOMI_INIT non sono un problema ordine di inserimento, con i fork degli atomi 
    //       che si chindono no ce modo di sapere come si comportera lo scheduler (interliving).
    //       Quindi utilizzo una ricerca seq che sta in O(n) pero non credo che dia fastidio 
    //       a nessuno spero :>. 
    AtomRecord tmp;
    for (size_t i = 0; i < n_atom; i++) {
        if (lseek(fd, i * sizeof(AtomRecord), SEEK_SET) == GENERIC_ERROR)
            gerrorm("Error seeking to record position");
        
        ssize_t rbytes = read(fd, &tmp, sizeof(AtomRecord));
        if (rbytes == -1 || rbytes != sizeof(AtomRecord)) 
            gerrorm("Error reading record from file");
        
        if (tmp.pid == pid)
            return i;
    }

    return -1;
}

// CRUD

// Function to write atom at index
void write_atom_record(size_t index, const AtomRecord atom) {
    fopen_check();

    // Goto the start byte + Write new record
    off_t fbyte = index * sizeof(AtomRecord);
    if (lseek(fd, fbyte, SEEK_SET) == -1)
        gerrorm("Error seeking to file position");
    
    ssize_t wbytes = write(fd, &atom, sizeof(AtomRecord));
    if (wbytes == -1 || wbytes != sizeof(AtomRecord)) 
        gerrorm("Error writing to file");
}

// Function to add new atom record
void add_atom_record(const AtomRecord atom) {
    fopen_check();

    // Goto the end file + Write new record 
    if (lseek(fd, 0, SEEK_END) == -1)
        gerrorm("Error seeking to end of file");

    ssize_t wbytes = write(fd, &atom, sizeof(AtomRecord));
    if (wbytes == -1 || wbytes != sizeof(AtomRecord)) 
        gerrorm("Error writing to file");
}

// Function to get record by pid + by
AtomRecord read_atom_record(const pid_t pid) {
    fopen_check();

    // Get index + Calc jump byte offset 
    int index = get_atom_index(pid); 
    if (index == -1)
        gerrorm("Error atom not found");
    
    off_t fbyte = index * sizeof(AtomRecord);
    if (lseek(fd, fbyte, SEEK_SET) == -1) 
        gerrorm("Error seeking to end of file");

    // Read + Return data
    AtomRecord tmp;
    ssize_t rbytes = read(fd, &tmp, sizeof(AtomRecord));
    if (rbytes == -1 || rbytes != sizeof(AtomRecord))
        gerrorm("Error reading to file");

    return tmp;
}

AtomRecord read_atom_record_i(size_t index) {
    off_t fbyte = index * sizeof(AtomRecord);
    if (lseek(fd, fbyte, SEEK_SET) == GENERIC_ERROR)
        gerrorm("Error seeking to end of file");

    // Read + Return data
    AtomRecord tmp;
    ssize_t rbytes = read(fd, &tmp, sizeof(AtomRecord));
    if (rbytes == -1 || rbytes != sizeof(AtomRecord))
        gerrorm("Error reading to file");

    return tmp;
}

// Print all db records
void print_all_db() {
    for (size_t i = 0; i < get_number_atom_records(); i++) {
        AtomRecord ar = read_atom_record_i(i);
        dprintf(
            STDOUT_FILENO, 
            "PID: %d, ISSLAG: %d, ATOM_NUMBER: %d\n", 
            ar.pid,
            ar.is_slag,
            ar.atom_number
        );
    }
}

// Function to close the database file
void close_db() {
    if (close(fd) == -1)
        gerrorm("Error closing file");
    fd = -1;
}