#ifndef ATOM_OPERATIONS_H
#define ATOM_OPERATIONS_H

#include <sys/types.h>

int distribution(int min, int max);
void init_atoms(int n, pid_t error_receiver);

#endif