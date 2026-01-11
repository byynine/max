#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <pcre2.h>

void substr(
    const char *src,
    size_t start,
    size_t len,
    char *dst
);

char *getstr(
    PCRE2_SIZE start,
    PCRE2_SIZE end,
    const char *org_input
);

int atnparse(
    int argc,
    char *argv[],
    int argi,
    int linei,
    char *cmd_str,
    size_t cmd_len,
    char *cmd
);

int parse(
    const char input[],
    size_t n_match,
    const char *pattern,
    PCRE2_SIZE *ovector
);

#endif
