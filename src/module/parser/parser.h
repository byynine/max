#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <regex.h>

int parse(    // parse a string by a regex pattern and output the match
    const char input[],
    size_t n_match,
    const char *pattern,
    regmatch_t *match
);

char *getstr(    // extract a match as a string from the original input
    regmatch_t match,
    char *org_input
);

#endif
