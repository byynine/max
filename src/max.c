#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

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

int main(int argc, char *argv[])
{
    FILE *maxfile = fopen("Maxfile", "r");
    if (!maxfile) { printf("Maxfile not found\n"); return 1; }
    
    for (int argi = 1; argi < argc; argi++)
    {
        // read Maxfile
        char line_buffer[1024];
        rewind(maxfile); // move back read cursor to file begining for each argument iteration

        while (fgets(line_buffer, sizeof(line_buffer), maxfile))
        {
            line_buffer[strcspn(line_buffer, "\n")] = '\0'; // line_buffer = "Hello, world!\0\0";
                                                            //                              ^
                                                            //                              replaced "\n"
           
            // define argument regex
            const char *pattern_arg = "^ *([a-zA-Z0-9 ]+)=([/@a-zA-Z0-9 .-]+)$";
            size_t n_match_arg = 3; // 1 for full, 2 for the submatches
            regmatch_t match_arg[n_match_arg];

            // parse argument regex
            int arg_res = parse(line_buffer, n_match_arg, pattern_arg, match_arg); 
            
            // matched
            if (!arg_res)
            {
                char *ref_str = getstr(match_arg[1], line_buffer);
                if (ref_str)
                {
                    if (!strcmp(ref_str, argv[argi])) // reference in Maxfile matches the argument
                    {
                        char *cmd_str = getstr(match_arg[2], line_buffer); // get the command that matches the reference
                        
                        const char *cmd_pattern = "@([0-9]+)";
                        regmatch_t cmd_match[2];
                        int offset = 0;

                        while (1)
                        {
                            int cmd_res = parse(cmd_str + offset, 2, cmd_pattern, cmd_match);
                            if (cmd_res == REG_NOMATCH) { break; }
                            else if (cmd_res != 0) { printf("parse error\n"); break; }

                            char *arg_num = getstr(cmd_match[1], cmd_str + offset);
                            printf("%s\n", arg_num);
                            free(arg_num);

                            offset += cmd_match[0].rm_eo;
                        }

                        if (cmd_str)
                        {
                            printf("%s\n", cmd_str);
                            system(cmd_str); // execute command written in Maxfile
                        }

                        free(cmd_str);
                    }

                    free(ref_str);
                }
            }
            else if (arg_res == REG_NOMATCH) { printf("no match in line buffer: %s\n", line_buffer); }
            else { printf("parse error\n"); return 1; }
        }
    }

    fclose(maxfile); 

    return 0;
}
