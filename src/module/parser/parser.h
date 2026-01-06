#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <regex.h>

int parse(
    const char inut[],
    size_t n_match,
    const char *pattern,
    regmatch_t *match
);

#endif
