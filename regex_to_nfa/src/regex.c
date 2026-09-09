/*

Input:Regex, Op={*,|,.}, precedencia de Op
Stack = []
Queue = []
c = forward()
while (c != eof)
{
    if (c not in Op) then
        Queue.push(c)
    else
        if(c = '()') then
            stack.push('(')
        else
            if (c = ')') then
                while (stack.pop() != '(')
                {
                    queue.push(stack.pop())
                }
                stack.pop()
            else
                if (c in Op) then
                    while(!stack.empty() && (stack.pop() != '(') && proc(stack.top) >= c)
                    {
                        queue.push(stack.pop())
                    }
                    queue.push(stack.pop())
    while (!stack.empty())
    {
        queue.push(stack.pop())
    }
    return queue
}
*/


#include "regex.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Agrega items al regex
void regex_add_item(Regex *r, char c) {
    if (r->size >= r->capacity) {
        r->capacity = r->capacity == 0 ? 10 : r->capacity * 2;
        r->items = (RegexItem *)realloc(r->items, sizeof(RegexItem) * r->capacity);
    }
    r->items[r->size].value = c;
    r->size++;
}

// Define la precedencia
int get_precedence(char c) {
    switch(c) {
        case '*': return 3;  // +
        case '.': return 2;
        case '|': return 1;  // -
        default: return 0;
    }
}

// Es operador?
int is_operator(char c) {
    return c == '*' || c == '|' || c == '.';
}


Regex parse_regex(const char *infix) {
    Regex result;
    result.items = NULL;
    result.size = 0;
    result.capacity = 0;
    
    Node *stack = NULL;  // Pila para operadores
    
    // Variables para la concatenación
    char prev_char = '\0';
    int i = 0;
    
    while (infix[i] != '\0') {
        char c = infix[i];
        
        // Lee la concatenación implícita
        if (i > 0 && 
            ((isalnum(prev_char) || prev_char == ')' || prev_char == '*') && 
             (isalnum(c) || c == '('))) {
            // Inserta operador de concatenación '.'
            char *dot = (char *)malloc(sizeof(char));
            *dot = '.';
            
            while (stack != NULL) {
                char *top_val = (char *)(stack->value);
                if (*top_val == '(') break;
                
                int prec_top = get_precedence(*top_val);
                int prec_dot = get_precedence('.');
                
                if (prec_top >= prec_dot) {
                    char *popped = (char *)pop(&stack);
                    regex_add_item(&result, *popped);
                    free(popped);
                } else {
                    break;
                }
            }
            
            push(&stack, dot);
        }
        
        if (isalnum(c)) {
            regex_add_item(&result, c);
        }
        else if (c == '(') {
            char *paren = (char *)malloc(sizeof(char));
            *paren = '(';
            push(&stack, paren);
        }
        else if (c == ')') {
            while (1) {
                char *top_val = (char *)pop(&stack);
                if (top_val == NULL) break;
                if (*top_val == '(') {
                    free(top_val);
                    break;
                }
                regex_add_item(&result, *top_val);
                free(top_val);
            }
        }
        else if (is_operator(c)) {
            // Verifica si es un operador unario '*'
            if (c == '*') {
                char *star = (char *)malloc(sizeof(char));
                *star = '*';
                
                // Procesa precedencia si hay operadores en la pila
                while (stack != NULL) {
                    char *top_val = (char *)(stack->value);
                    if (*top_val == '(') break;
                    
                    int prec_top = get_precedence(*top_val);
                    int prec_star = get_precedence('*');
                    
                    if (prec_top >= prec_star) {
                        char *popped = (char *)pop(&stack);
                        regex_add_item(&result, *popped);
                        free(popped);
                    } else {
                        break;
                    }
                }
                
                push(&stack, star);
            } else {
                // Operadores binarios
                while (stack != NULL) {
                    char *top_val = (char *)(stack->value);
                    if (*top_val == '(') break;
                    
                    int prec_top = get_precedence(*top_val);
                    int prec_c = get_precedence(c);
                    
                    if (prec_top >= prec_c) {
                        char *popped = (char *)pop(&stack);
                        regex_add_item(&result, *popped);
                        free(popped);
                    } else {
                        break;
                    }
                }
                
                char *op = (char *)malloc(sizeof(char));
                *op = c;
                push(&stack, op);
            }
        }
        
        prev_char = c;
        i++;
    }
    
    // Vacía la pila
    while (stack != NULL) {
        char *val = (char *)pop(&stack);
        if (val != NULL) {
            regex_add_item(&result, *val);
            free(val);
        }
    }
    
    return result;
}

// Libera memoria del regex
void free_regex(Regex *r) {
    if (r->items) {
        free(r->items);
        r->items = NULL;
    }
    r->size = 0;
    r->capacity = 0;
}