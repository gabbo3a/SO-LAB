#ifndef MSGQM_H
#define MSGQM_H

// Messaggio generico architettura
// Nota: ...
typedef struct {
    long mtype;     // Pid del ricevente (Unico modo per filtrare i msg)
    long sender;    // Pid del mittente  (Utile per conflitti di gerarchia, priorita varie)
    long sreciver;  // Pid del ricevente di secondo grado (Utile per "rimbalzare i msg")
    int raction;    // Tipo di azione per ricevente
} GAMessage;

#define GENERIC_ARCHITETURE_MESSAGE_SIZE sizeof(GAMessage) - sizeof(long) // ???

enum {
    ATOM_SPILITTING_ACTION, 
    ATOM_DEATH_ACTION, 
    INHIBITOR_ASK_PERMISSION
};

void init_msgq();
void close_msgq();
void clear_mtype_msgq(long mtype);
GAMessage build_msg_atom(pid_t atom);
GAMessage build_msg_inhibitor(pid_t inhibitor, pid_t atom);
void send_msg(GAMessage);
GAMessage recive_msg_wait(long, bool);

#endif