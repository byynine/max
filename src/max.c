#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "module/cmdopts/cmdopts.h"
#include "module/parser/parser.h"
#include "module/usage/usage.h"

// default Maxfile path
char *maxfile_path = "Maxfile";

// define argument regex pattern
const char *pattern_arg = "^ *([a-zA-Z0-9 ]+)=([/@a-zA-Z0-9 .-]+)$";
size_t n_match_arg = 3; // 1 for full, 2 for the submatches

int main(int argc, char *argv[])
{    
    if (argc == 1) { printf("%s", usage); return 1; }

    // command-line options
    int resopts = cmdopts(argc, argv, &maxfile_path);
    if (resopts == 3) { return 0; }
    else if (resopts == 2) { return 0; }
    else if (resopts != 0) { return 1; }

    FILE *maxfile = fopen(maxfile_path, "r");
    if (!maxfile) { printf("Maxfile not found\n"); return 1; }
    
    for (int argi = 1; argi < argc; argi++)
    {
        // read Maxfile
        char line_buffer[1024];
        rewind(maxfile); // move back read cursor to file begining for each argument iteration
        
        unsigned int linei = 1; // line index counter for printing purposes

        while (fgets(line_buffer, sizeof(line_buffer), maxfile))
        {
            line_buffer[strcspn(line_buffer, "\n")] = '\0'; // replace \n with null terminator

            // parse argument regex
            regmatch_t match_arg[n_match_arg];
            int arg_res = parse(line_buffer, n_match_arg, pattern_arg, match_arg); 

            if (arg_res == REG_NOMATCH) { printf("no match in line buffer: %s\n", line_buffer); continue; } // no match
            else if (arg_res) { printf("parse error\n"); return 1; } // parse error

            char *ref_str = getstr(match_arg[1], line_buffer); // turn match_arg[1] into string
            if (!ref_str) { printf("error getting reference string"); return 1; }

            // compare reference and current argument. if it's not 0 (matched) then skip
            if (strcmp(ref_str, argv[argi])) { continue; }

            char *cmd_str = getstr(match_arg[2], line_buffer); // get the command that matches the reference
            size_t cmd_len = strlen(cmd_str);

            char cmd[1024]; // command that will be executed
            cmd[0] = '\0';

            // @n parse on cmd
            atnparse(argc, argv, argi, linei, cmd_str, cmd_len, cmd);

            // print and execute the final constructed command
            printf("%s\n", cmd);
            system(cmd);

            free(cmd_str);
            free(ref_str);
            
            linei++;
        }
    }

    fclose(maxfile); 

    return 0;
}
