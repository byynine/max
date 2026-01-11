
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre2.h>

#include "parser.h"

void substr(
    const char *src,
    size_t start,
    size_t len,
    char *dst
)
{
    memcpy(dst, src + start, len);
    dst[len] = '\0';
}

char *getstr(PCRE2_SIZE start, PCRE2_SIZE end, const char *org_input)
{
    size_t len = end - start;
    char *match_str = malloc(len + 1);
    if (!match_str) return NULL;

    memcpy(match_str, org_input + start, len);
    match_str[len] = '\0';
    return match_str;
}

int atnparse(
    int argc,
    char *argv[],
    int argi,
    int linei,
    char *cmd_str,
    size_t cmd_len,
    char *cmd
)
{
    const char *cmd_pattern = "@(-?[0-9]+)";
    size_t n_cmd_match = 2; // full match + submatch

    size_t offset = 0;
    char cmd_sub[cmd_len + 1];
    memcpy(cmd_sub, cmd_str, cmd_len + 1);

    while (1)
    {
        PCRE2_SIZE ovector[n_cmd_match * 3] = {0};
        int cmd_res = parse(cmd_str + offset, n_cmd_match, cmd_pattern, ovector);
        if (cmd_res == REG_NOMATCH) break;
        else if (cmd_res != 0) { printf("parse error\n"); break; }

        char *arg_num = getstr(ovector[2], ovector[3], cmd_str + offset);
        int abs_arg_num = argi + (int)strtol(arg_num, NULL, 10);
        char abs_arg_buf[32];

        if (abs_arg_num < 0 || abs_arg_num >= argc)
        {
            printf("line %d: argument out of range\n", linei);
            free(arg_num);
            return 1;
        }

        sprintf(abs_arg_buf, "%s", argv[abs_arg_num]);

        char cmd_part[ovector[0] + 1];
        substr(cmd_sub, offset, ovector[0], cmd_part);

        strcat(cmd, cmd_part);
        strcat(cmd, abs_arg_buf);

        free(arg_num);

        offset += ovector[1];
    }

    // append remaining part after last @n
    size_t end_len = strlen(cmd_sub) - offset;
    char cmd_part[end_len + 1];
    substr(cmd_sub, offset, end_len, cmd_part);
    strcat(cmd, cmd_part);

    return 0;
}

int parse(
    const char input[],
    size_t n_match,
    const char *pattern,
    PCRE2_SIZE *ovector
)
{
    int errorcode;
    PCRE2_SIZE erroffset;

    pcre2_code *re = pcre2_compile(
        (PCRE2_SPTR)pattern,
        PCRE2_ZERO_TERMINATED,
        0,
        &errorcode,
        &erroffset,
        NULL
    );

    if (!re) { printf("regex compile failure\n"); return 1; }

    int rc = pcre2_match(
        re,
        (PCRE2_SPTR)input,
        strlen(input),
        0,
        0,
        ovector,
        n_match * 3
    );

    pcre2_code_free(re);

    if (rc < 0)
    {
        if (rc == PCRE2_ERROR_NOMATCH) return REG_NOMATCH;
        return 1;
    }

    return rc;
}
