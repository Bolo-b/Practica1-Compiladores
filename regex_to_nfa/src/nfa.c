#include "regex.h"
#include "nfa.h"
#include "stack.h"

#include <stdlib.h>

//Used in Thompson for saving the start and finish of the nfa
typedef struct 
{
    int fst;
    int snd;
} pair;


nfa regex_to_nfa(regex r)
{   
    int size_regex = r.length;
    int total_transitions = 0;
    //Count the transitions and states of the future nfa
    for (int i = 0; i < size_regex; i++)
    {
        switch(r.regex_symbols[i])
        {
        case '*':
            total_transitions = total_transitions + 4;
            break;
        case '|':
            total_transitions = total_transitions + 4;
            break;
        case '.':
            total_transitions++;
            break;
        default:
            total_transitions++;
            break;
        }
    }
    
    //Implementation of Thompson's algorithm
    Node *stack = NULL;
    int avaiable_state = 0;
    transition *transitions = malloc(sizeof(transition) * total_transitions);
    int actual_transition = 0;

    for (int i = 0; i < size_regex; i++)
    {
        switch(r.regex_symbols[i])
        {
        case '*':
            {
            //take just one nfa from the stack
            pair *p1 = (pair *)pop(&stack);
            //values of the 2 new states
            int state1 = avaiable_state;
            int state2 = avaiable_state+1;
            avaiable_state = avaiable_state + 2;
            //transitions since state1 to first of p1 and second of p1 to state2 
            transition t1 = {state1,p1->fst,EPSILON};
            transitions[actual_transition] = t1; 
            actual_transition++;
            transition t2 = {p1->snd,state2,EPSILON};
            transitions[actual_transition] = t2; 
            actual_transition++;
            //transition since state1 to state2, (just skipping)
            transition t3 = {state1,state2,EPSILON};
            transitions[actual_transition] = t3; 
            actual_transition++;
            //transition since second of p1 to first of p1 to let cycle
            transition t4 = {p1->snd,p1->fst,EPSILON};
            transitions[actual_transition] = t4; 
            actual_transition++;
            //take the new start and accept states and added to the stack
            pair *p = malloc(sizeof(pair));
            p->fst=state1;
            p->snd=state2;
            push(&stack,p);
            //free memory of the pair removed from the stack
            free(p1);
            break;
            }
        case '|':
            {
            //take the begin and finish of the nfas from the stack
            pair *p2 = (pair *)pop(&stack);
            pair *p1 = (pair *)pop(&stack);
            //values of the 2 new states
            int state1 = avaiable_state;
            int state2 = avaiable_state+1;
            avaiable_state = avaiable_state + 2;
            //transitions since state1 to first of p1 and first of p2
            transition t1 = {state1,p1->fst,EPSILON};
            transitions[actual_transition] = t1; 
            actual_transition++;
            transition t2 = {state1,p2->fst,EPSILON};
            transitions[actual_transition] = t2; 
            actual_transition++;
            //transitions since the second of p1 and second of p2 to state2
            transition t3 = {p1->snd,state2,EPSILON};
            transitions[actual_transition] = t3; 
            actual_transition++;
            transition t4 = {p2->snd,state2,EPSILON};
            transitions[actual_transition] = t4; 
            actual_transition++;
            //take the new start and accept states and added to the stack
            pair *p = malloc(sizeof(pair));
            p->fst=state1;
            p->snd=state2;
            push(&stack,p);
            //free memory of the pairs removed from the stack
            free(p1);
            free(p2);
            break;
            }
        case '.':
            {
            pair *p2 = (pair *)pop(&stack);
            pair *p1 = (pair *)pop(&stack);
            //transition of the second of p1 and the first of p2
            transition t = {p1->snd,p2->fst,EPSILON};
            transitions[actual_transition] = t; 
            actual_transition++;
            //take the new start and accept states and added to the stack
            pair *p = malloc(sizeof(pair));
            p->fst=p1->fst;
            p->snd=p2->snd;
            push(&stack,p);
            //free memory of the pairs removed from the stack
            free(p1);
            free(p2);
            break;
            }
        default:
            {
            int state1 = avaiable_state;
            int state2 = avaiable_state+1;
            avaiable_state = avaiable_state + 2;
            transition t = {state1,state2,r.regex_symbols[i]};
            transitions[actual_transition] = t;
            actual_transition++;
            //Insert the initial and final state of the nfa to the stack
            pair *p = malloc(sizeof(pair));
            p->fst=state1;
            p->snd=state2;
            push(&stack,p);
            break;
            }
        }
    }

    pair *p = (pair *)pop(&stack);
    nfa final_nfa = {transitions,p->fst,p->snd,total_transitions,actual_transition};
    free(p);
    return final_nfa;
}
