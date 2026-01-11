#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre2.h>

#include "module/cmdopts/cmdopts.h"
#include "module/parser/parser.h"
#include "module/usage/usage.h"

// default Maxfile path
char *maxfile_path = "Maxfile";

// define argument regex pattern
const char *pattern_arg = "^ *([a-zA-Z0-9 ]+)=([/@a-zA-Z0-9 .-]+)$";
size_t n_match_arg = 3; // 1 for full match, 2 for submatches

int main(int argc, char *argv[])
{
    int resopts = cmdopts(argc, argv, &maxfile_path);
    if (resopts == 3 || resopts == 2) return 0;
    else if (resopts != 0) return 1;

    FILE *maxfile = fopen(maxfile_path, "r");
    if (!maxfile) { printf("Maxfile not found\n"); return 1; }

    for (int argi = 0; argi < argc; argi++)
    {
        if (argi == 0 && argc != 1) continue;

        char line_buffer[1024];
        rewind(maxfile);

        unsigned int linei = 1;

        while (fgets(line_buffer, sizeof(line_buffer), maxfile))
        {
            line_buffer[strcspn(line_buffer, "\n")] = '\0';

            // prepare ovector for PCRE2 matches
            PCRE2_SIZE ovector[n_match_arg * 3] = {0};
            int arg_res = parse(line_buffer, n_match_arg, pattern_arg, ovector);

            if (arg_res == REG_NOMATCH) { printf("no match in line buffer: %s\n", line_buffer); continue; }
            else if (arg_res) { printf("parse error\n"); return 1; }

            char *ref_str = getstr(ovector[2], ovector[3], line_buffer); // first submatch
            if (!ref_str) { printf("error getting reference string\n"); return 1; }

            if (strcmp(ref_str, argv[argi])) { free(ref_str); continue; }

            char *cmd_str = getstr(ovector[4], ovector[5], line_buffer); // second submatch
            size_t cmd_len = strlen(cmd_str);

            char cmd[1024] = {0};

            atnparse(argc, argv, argi, linei, cmd_str, cmd_len, cmd);

            printf("%s\n", cmd);
            system(cmd);

            free(ref_str);
            free(cmd_str);

            linei++;
        }
    }

    fclose(maxfile);

    return 0;
}
