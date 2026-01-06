#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "module/cmdopts/cmdopts.h"
#include "module/parser/parser.h"

// default Maxfile path
char *maxfile_path = "Maxfile";

// define argument regex pattern
const char *pattern_arg = "^ *([a-zA-Z0-9 ]+)=([/@a-zA-Z0-9 .-]+)$";
size_t n_match_arg = 3; // 1 for full, 2 for the submatches

int main(int argc, char *argv[])
{    
    // command-line options
    int resopts = cmdopts(argc, argv, &maxfile_path);
    if (resopts == 2) { return 0; }
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
            line_buffer[strcspn(line_buffer, "\n")] = '\0'; // line_buffer = "Hello, world!\0\0";
                                                            //                              ^
                                                            //                              replaced "\n"

            // parse argument regex
            regmatch_t match_arg[n_match_arg];
            int arg_res = parse(line_buffer, n_match_arg, pattern_arg, match_arg); 
            
            // matched
            if (!arg_res)
            {
                char *ref_str = getstr(match_arg[1], line_buffer);
                if (ref_str)
                {
                    if (!strcmp(ref_str, argv[argi])) // reference in Maxfile matches the argument
                    {
                        char cmd[1024]; // command that will be executed
                        cmd[0] = '\0';

                        char *cmd_str = getstr(match_arg[2], line_buffer); // get the command that matches the reference
                        size_t cmd_len = strlen(cmd_str);

                        // @n parse
                        atnparse(argc, argv, argi, linei, cmd_str, cmd_len, cmd);

                        // print and execute the final constructed command
                        printf("%s\n", cmd);
                        system(cmd);

                        free(cmd_str);
                    }

                    free(ref_str);
                }
            }
            else if (arg_res == REG_NOMATCH) { printf("no match in line buffer: %s\n", line_buffer); }
            else { printf("parse error\n"); return 1; }
            
            linei++;
        }
    }

    fclose(maxfile); 

    return 0;
}
