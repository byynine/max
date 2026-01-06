#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <regex.h>

void substr(    // extract a string from a string
    const char *src,
    size_t start,
    size_t len,
    char *dst
);

char *getstr(    // extract a match as a string from the original input
    regmatch_t match,
    char *org_input
);

int atnparse(    // perform @n parse on cmd
    int argc,
    char *argv[],
    int argi,
    int linei,
    char *cmd_str,
    size_t cmd_len,
    char *cmd
);

int parse(    // parse a string by a regex pattern and output the match
    const char input[],
    size_t n_match,
    const char *pattern,
    regmatch_t *match
);

#endif
