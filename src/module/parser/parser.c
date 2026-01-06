#include <stdio.h>
#include <regex.h>

#include "parser.h"

int parse(    // parse a string by a regex pattern and output the match
    const char input[],
    size_t n_match,
    const char *pattern,
    regmatch_t *match
)
{
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED)) { printf("regex compile failure\n"); return 1; }
    int res = regexec(&re, input, n_match, match, 0);
    regfree(&re);

    return res;
}
