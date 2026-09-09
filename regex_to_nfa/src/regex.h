typedef struct {
    char *regex_symbols;
    int length;
} regex;

#ifndef REGEX_H
#define REGEX_H


// Estructura para los items del regex
typedef struct {
    char value;
} RegexItem;

// Estructura del regex (notación postfija)
typedef struct {
    RegexItem *items;
    int size;
    int capacity;
} Regex;

// Función principal que convierte infijo a postfijo
Regex parse_regex(const char *infix);

#endif