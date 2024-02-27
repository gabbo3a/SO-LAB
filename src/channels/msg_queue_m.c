#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "utility.h"
#include "msg_queue_m.h"

#define GAMSIZE GENERIC_ARCHITETURE_MESSAGE_SIZE

// Explanation: Modulo di gestione della coda di msg come per il modulo di gestione del db incaspula le 
//              informazioni necessarie. Come tutti i moduli per IPC system v implemenati per questo progetto
//              utilizza ftok con con una coppia path lettera sempr uguali per far si che tutti i processi che
//              condividano la working dir utilizzino le stesse IPC.
// Nota:        Questo non è un problema in generale poiche il progetto non è pensato per avere piu simulazioni
//              nella stessa working dir.

static const char* name = ".";
static int msgq_id      = -1;

// Function to init msg queue
void init_msgq() {
    // Create key + Create msg queue
    key_t key = ftok(name, 'Q');
    if (key == GENERIC_ERROR)
        gerrorm("Error generating IPC key");

    msgq_id = msgget(key, IPC_CREAT | 0666);
    if (msgq_id == GENERIC_ERROR) 
        gerrorm("Error creating message queue");
}

// Function to clear by mtype
void clear_mtype_msgq(long mtype) {
    GAMessage tmp;
    while (msgrcv(msgq_id, &tmp, GAMSIZE, mtype, IPC_NOWAIT) != GENERIC_ERROR);
}

// Functions to build different types of messages
GAMessage build_msg_atom(pid_t atom) {
    return (GAMessage) {
        atom, 
        getpid(), 
        0, 
        ATOM_SPILITTING_ACTION
    };
}

GAMessage build_msg_inhibitor(pid_t inhibitor, pid_t atom) {
    return (GAMessage) {
        inhibitor, 
        getpid(), 
        atom, 
        INHIBITOR_ASK_PERMISSION
    };
}

// Function to send message 
void send_msg(GAMessage msg) {
    // dprintf(1, "msg: [%ld, %ld, %ld, %d]\n", msg.mtype, msg.sender, msg.sreciver , msg.raction);

    int cflag = msgsnd(msgq_id, &msg, GAMSIZE, 0);
    if (cflag == GENERIC_ERROR)
        gerrorm("Error sending message");
}

// Function to receive msg via receiver PID
GAMessage recive_msg_wait(long mtype, bool lerror) {
    GAMessage tmp; // memset(&tmp, 0, sizeof(GAMessage));
    
    int cflag = msgrcv(msgq_id, &tmp, GAMSIZE, mtype, 0);
    if (lerror && cflag == GENERIC_ERROR)
        gerrorm("Error receiving message");

    return tmp;
}

// Function to close + remove msg queue
void close_msgq() {
    int cflag = msgctl(msgq_id, IPC_RMID, NULL);
    if (cflag == GENERIC_ERROR)
        gerrorm("Error removing message queue");    
}