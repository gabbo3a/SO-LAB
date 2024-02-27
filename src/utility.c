#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "utility.h"

// Function to cast numeber to heap string
const char* ntos(const int n) {
    int length = (int) floor(log10(n)) + 1;    // Get number digit (Fonte: leetcode :>)

    // Memory allocation + handle error
    // Nota (Borbotio): in Java era tutto una 
    //                  riga da qui fino al return :(
    char* cm  = (char*) malloc(length * sizeof(char));
    if (cm == NULL) 
        gerrorm("Memory allocation error");

    sprintf(cm, "%d", n);
    return cm;
}

// Function to get random number between min/max
int get_lrand(int min, int max) {
    if (min > max) gerrorm("min > max");

    static unsigned int seed = 0;                           // Seed contatore (Fonte: slide)
    const int n = min + rand_r(&seed) % (max - min + 1);    // Generate atom number
    seed++;                                                 // Change seed
    return n;
} 

// Function to check if value in array
bool contain(int* array, size_t size, int value) {
    for (size_t i = 0; i < size; i++) 
        if (array[i] == value) 
            return true;
    return false;
}

sigset_t* get_struct_block_all_signal() {
    sigset_t* all_signals = malloc(sizeof(sigset_t));
    if (all_signals == NULL) 
        gerrorm("Error in alloc memory");

    sigfillset(all_signals);
    return all_signals;
}