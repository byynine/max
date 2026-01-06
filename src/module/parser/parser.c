#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char *getstr(    // extract a match as a string from the original input
    regmatch_t match,
    char *org_input
)
{
    int len = match.rm_eo - match.rm_so;
    
    char *match_str = malloc(len + 1);
    if (!match_str) { return NULL; }

    memcpy(match_str, org_input + match.rm_so, len);
    match_str[len] = '\0';
    
    return match_str;
}
