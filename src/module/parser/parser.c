#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "parser.h"

void substr(    // extract a string from a string
    const char *src,
    size_t start,
    size_t len,
    char *dst
)
{
    memcpy(dst, src + start, len);
    dst[len] = '\0';
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

int atnparse(    // perform @n parse on cmd
    int argc,
    char *argv[],
    int argi,
    int linei,
    char *cmd_str,
    size_t cmd_len,
    char *cmd
)
{
    const char *cmd_pattern = "@([0-9]+)";
    size_t n_cmd_match = 2;

    // @n feature
    // uses substr()
    // needs input_str, pattern, argv, output_str 
    regmatch_t cmd_match[n_cmd_match];
    size_t offset = 0;

    char cmd_sub[cmd_len + 1];
    memcpy(cmd_sub, cmd_str, cmd_len + 1);

    // iteration for finding @n and concatenating it
    while (1)
    {
        int cmd_res = parse(cmd_str + offset, n_cmd_match, cmd_pattern, cmd_match);
        if (cmd_res == REG_NOMATCH) { break; }
        else if (cmd_res != 0) { printf("parse error\n"); break; }

        char *arg_num = getstr(cmd_match[1], cmd_str + offset);
        int abs_arg_num = argi + (int)strtol(arg_num, NULL, 10);
        char abs_arg_buf[12];

        if (abs_arg_num >= argc) { printf("line %d: argument out of range\n", linei); return 1; }
        sprintf(abs_arg_buf, "%s", argv[abs_arg_num]);

        char cmd_part[cmd_match[0].rm_so];
        substr(cmd_sub, offset, cmd_match[0].rm_so, cmd_part);

        strcat(cmd, cmd_part);
        strcat(cmd, abs_arg_buf);

        free(arg_num);

        offset += cmd_match[0].rm_eo;
    }

    // @n iteration doesnt include the last cmd_part, so do one more after the iteration ends
    size_t end_len = strlen(cmd_sub) - offset;
    char cmd_part[end_len + 1]; // if this ends with a @n it includes a newline

    substr(cmd_sub, offset, end_len, cmd_part);
    strcat(cmd, cmd_part);

    return 0;
}

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

