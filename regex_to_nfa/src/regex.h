#ifndef REGEX_H
#define REGEX_H

typedef struct {
    char *regex_symbols;
    int length;
} regex;

regex parse_regex(const char *infix);
void free_regex(regex *r);

#endif