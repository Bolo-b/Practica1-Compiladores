#ifndef NFA_H
#define NFA_H

#include <stdbool.h>

#define EPSILON '\0'

//Diferent states are represented just as a positive integer 
typedef struct transition {
    int start;
    int finish;
    char symbol; //the symbol needed to travel between states
} transition;

//We will represent the nfa as a list of transitions
typedef struct nfa {
    transition *transitions;
    int start;
    int accept;
    int length;
    int states;
} nfa;

// Functions for NFA simulation and management
nfa regex_to_nfa(regex r);
int epsilon_closure(const nfa *n, const int *current_states, int current_count, int *out_closure, int max_states);
int match_nfa(nfa n, char *buf, int len);
void free_nfa(nfa *n);
bool save_nfa(const nfa *n, const char *output_path);

#endif

