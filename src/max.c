#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char version[] = "v1.0.0d";

// default Maxfile path, modifiable by the -m/--maxfile option
char *maxfile_path = "Maxfile";

// define argument regex pattern
const char *pattern_arg = "^ *([a-zA-Z0-9 ]+)=([/@a-zA-Z0-9 .-]+)$";
size_t n_match_arg = 3; // 1 for full, 2 for the submatches

// @n pattern
const char *cmd_pattern = "@([0-9]+)";
size_t n_cmd_match = 2;

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

void substr(const char *src, size_t start, size_t len, char *dst)
{
    memcpy(dst, src + start, len);
    dst[len] = '\0';
}

int cmdopts(int argc, char *argv[], char **maxfile_path)
{
    for (int argi = 0; argi < argc; argi++)
    {
        if (strcmp(argv[argi], "-m") == 0 || strcmp(argv[argi], "--maxfile") == 0)
        {
            if (argi + 1 < argc)
            {
                *maxfile_path = argv[argi + 1];
            }
            else { printf("argument option missing\n"); return 1; }
        }
        else if (strcmp(argv[argi], "-v") == 0 || strcmp(argv[argi], "--version") == 0)
        {
            printf("%s\n", version);
            return 2;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{    
    int resopts = cmdopts(argc, argv, &maxfile_path);
    if (resopts) { return 1; }
    else if (resopts == 2) { return 0; }

    FILE *maxfile = fopen(maxfile_path, "r");
    if (!maxfile) { printf("Maxfile not found\n"); return 1; }
    
    for (int argi = 1; argi < argc; argi++)
    {
        // read Maxfile
        char line_buffer[1024];
        rewind(maxfile); // move back read cursor to file begining for each argument iteration
        
        unsigned int linei = 1;

        while (fgets(line_buffer, sizeof(line_buffer), maxfile))
        {
            line_buffer[strcspn(line_buffer, "\n")] = '\0'; // line_buffer = "Hello, world!\0\0";
                                                            //                              ^
                                                            //                              replaced "\n"

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
                        char cmd[1024];
                        cmd[0] = '\0';

                        char *cmd_str = getstr(match_arg[2], line_buffer); // get the command that matches the reference
                        size_t cmd_len = strlen(cmd_str);

                        // @n feature
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
